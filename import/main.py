from config import get_city
from graph_utils import save_road_graph, extract_and_save_graph_data

def main():
    city = get_city()
    print(f"Configuration loaded. City: {city}")
    
    nodes_json, edges_json = save_road_graph(city)
    print("Road graph saved as separate JSON files.")
    
    extract_and_save_graph_data(nodes_json, edges_json)
    print("Data extraction and file writing completed.")

if __name__ == "__main__":
    main()