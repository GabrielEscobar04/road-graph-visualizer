import osmnx as ox
import json
import math
from collections import defaultdict

def save_road_graph(location):
    graph = ox.graph_from_place(location, network_type='drive')
    nodes, edges = ox.graph_to_gdfs(graph, nodes=True, edges=True, node_geometry=True, fill_edge_geometry=True)
    
    nodes_json = json.loads(nodes.to_json())
    edges_json = json.loads(edges.to_json())
    
    with open("data/json/nodes.json", "w") as file:
        json.dump(nodes_json, file, indent=2)
    
    with open("data/json/edges.json", "w") as file:
        json.dump(edges_json, file, indent=2)

    return nodes_json, edges_json

def haversine_distance(latitude1, longitude1, latitude2, longitude2):
    earth_radius_meters = 6371000
    
    latitude1, longitude1, latitude2, longitude2 = map(math.radians, [latitude1, longitude1, latitude2, longitude2])
    
    dlatitude = latitude2 - latitude1
    dlongitude = longitude2 - longitude1
    a = math.sin(dlatitude/2)**2 + math.cos(latitude1) * math.cos(latitude2) * math.sin(dlongitude/2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))
    
    return earth_radius_meters * c

def center_and_convert_coordinates(coordinates):
    longitudes, latitudes = zip(*coordinates)
    center_longitude = (min(longitudes) + max(longitudes)) / 2
    center_latitude = (min(latitudes) + max(latitudes)) / 2
    
    centered_coords = [(longitude - center_longitude, latitude - center_latitude) for longitude, latitude in coordinates]
    
    return [
        (
            math.copysign(haversine_distance(center_latitude, center_longitude, center_latitude, longitude + center_longitude), longitude),
            math.copysign(haversine_distance(center_latitude, center_longitude, latitude + center_latitude, center_longitude), latitude)
        )
        for longitude, latitude in centered_coords
    ]

def extract_and_save_graph_data(nodes_json, edges_json):
    node_id_mapping = {}
    coordinates = [node['geometry']['coordinates'] for node in nodes_json['features']]
    
    meter_coords = center_and_convert_coordinates(coordinates)
    
    avg_speeds = defaultdict(list)
    
    def round_speed(speed):
        return round(speed / 10) * 10
    
    def estimate_speed(highway_type, properties):
        if isinstance(highway_type, list):
            highway_type = highway_type[0]
        
        if highway_type == 'motorway':
            return 120
        elif highway_type in ['trunk', 'primary']:
            return 90
        elif highway_type in ['secondary', 'tertiary']:
            return 50
        elif highway_type == 'residential':
            return 30
        else:
            return 40
    
    def parse_speed(speed_value):
        if speed_value is None:
            return None
        if isinstance(speed_value, (int, float)):
            return int(speed_value)
        if isinstance(speed_value, str):
            try:
                return int(speed_value)
            except ValueError:
                if speed_value.lower() == 'walk':
                    return 5  # Assuming walking speed is 5 km/h
                return None
        if isinstance(speed_value, list):
            parsed_speeds = [parse_speed(s) for s in speed_value]
            return sum(filter(None, parsed_speeds)) / len(list(filter(None, parsed_speeds))) if any(parsed_speeds) else None
        return None

    with open('data/nodes.txt', 'w') as nodes_file, open('data/edges.txt', 'w') as edges_file:
        for index, (node, (x, y)) in enumerate(zip(nodes_json['features'], meter_coords)):
            original_id = node['id']
            node_id_mapping[original_id] = index
            nodes_file.write(f"{index} {x:.6f} {y:.6f} 0\n")

        for edge in edges_json['features']:
            ids = edge['id'].strip("()").split(", ")
            new_node1_id = node_id_mapping[ids[0]]
            new_node2_id = node_id_mapping[ids[1]]

            properties = edge['properties']
            length = round(properties.get("length", 0), 3)

            highway_type = properties.get("highway", "unclassified")
            if isinstance(highway_type, list):
                highway_type = highway_type[0]
            
            maxspeed = parse_speed(properties.get("maxspeed"))
            if maxspeed is None:
                if avg_speeds[highway_type]:
                    maxspeed = round_speed(sum(avg_speeds[highway_type]) / len(avg_speeds[highway_type]))
                else:
                    maxspeed = estimate_speed(highway_type, properties)
            
            maxspeed = round_speed(maxspeed)
            avg_speeds[highway_type].append(maxspeed)

            lanes = properties.get("lanes")
            if lanes is None:
                lanes = 2
            elif isinstance(lanes, list):
                lanes = max(map(int, lanes))
            else:
                try:
                    lanes = int(lanes)
                except ValueError:
                    lanes = 2

            oneway = 1 if str(properties.get('oneway', 'Unknown')).lower() == "true" else 0

            edges_file.write(f"{new_node1_id} {new_node2_id} {length} {maxspeed} {lanes} {oneway}\n")

    return avg_speeds