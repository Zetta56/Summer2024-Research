#include <iostream>
#include <chrono>
#include "hnsw.h"

using namespace std;

/** 
 * This class is used to run a single instance of the HNSW algorithm.
*/
int main() {

    
    // Initialize time and config
    auto begin_time = chrono::high_resolution_clock::now();
    time_t now = time(NULL);
    cout << "HNSW run started at " << ctime(&now);
    Config* config = new Config();

    // Construct HNSW
    float** nodes = new float*[config->num_nodes];
    load_nodes(config, nodes);
    cout << "Beginning HNSW construction" << endl;
    HNSW* hnsw = init_hnsw(config, nodes);
    if (config->load_graph_file) {
        load_hnsw_file(config, hnsw, nodes);
    } else {
        for (int i = 1; i < config->num_nodes; i++) {
            hnsw->insert(config, i);
        }
    }

    // Print and export HNSW graph
    if (config->print_graph) {
        cout << hnsw;
    }
    if (config->export_graph) {
        hnsw->export_graph(config);
    }

    // Run queries
    if (config->run_search) {
        // Generate num_queries amount of queries
        float** queries = new float*[config->num_queries];
        load_queries(config, nodes, queries);
        auto search_start = chrono::high_resolution_clock::now();
        cout << "Time passed: " << chrono::duration_cast<chrono::milliseconds>(search_start - begin_time).count() << " ms" << endl;
        cout << "Beginning search" << endl;

        // Run query search and print results
        hnsw->search_queries(config, queries);

        auto search_end = chrono::high_resolution_clock::now();
        cout << "Time passed: " << chrono::duration_cast<chrono::milliseconds>(search_end - search_start).count() << " ms" << endl;

        // Delete queries
        for (int i = 0; i < config->num_queries; ++i)
            delete[] queries[i];
        delete[] queries;
    }

    // Clean up
    for (int i = 0; i < config->num_nodes; i++)
        delete[] nodes[i];
    delete[] nodes;
    delete hnsw;
    delete config;

    // Print time elapsed
    now = time(NULL);
    cout << "HNSW run ended at " << ctime(&now);
    auto end_time = chrono::high_resolution_clock::now();
    cout << "Total time taken: " << chrono::duration_cast<chrono::milliseconds>(end_time - begin_time).count() << " ms" << endl;

    return 0;
}
