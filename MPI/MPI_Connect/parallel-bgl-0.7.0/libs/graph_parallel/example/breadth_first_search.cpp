// Copyright (C) 2004-2008 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine

// Example usage of breadth_first_search algorithm

// Enable PBGL interfaces to BGL algorithms
#include <boost/graph/use_mpi.hpp>

// Communicate via MPI
#include <boost/graph/distributed/mpi_process_group.hpp>

// Breadth-first search algorithm
#include <boost/graph/breadth_first_search.hpp>

// Distributed adjacency list
#include <boost/graph/distributed/adjacency_list.hpp>

// METIS Input
#include <boost/graph/metis.hpp>

// Graphviz Output
#include <boost/graph/distributed/graphviz.hpp>

// Standard Library includes
#include <fstream>
#include <string>

#ifdef BOOST_NO_EXCEPTIONS
void
boost::throw_exception(std::exception const& ex)
{
    std::cout << ex.what() << std::endl;
    abort();
}
#endif

using namespace boost;
using boost::graph::distributed::mpi_process_group;

/* An undirected graph with distance values stored on the vertices. */
typedef adjacency_list<vecS, distributedS<mpi_process_group, vecS>, undirectedS,
                       /*Vertex properties=*/property<vertex_distance_t, std::size_t> >
  Graph;

int main(int argc, char* argv[])
{
  boost::mpi::environment env(argc,argv);

  // Parse command-line options
  const char* filename = "weighted_graph.gr";
  if (argc > 1) filename = argv[1];

  // Open the METIS input file
  std::ifstream in(filename);
  graph::metis_reader reader(in);

  // Load the graph using the default distribution
  Graph g(reader.begin(), reader.end(),
          reader.num_vertices());

  // Get vertex 0 in the graph
  graph_traits<Graph>::vertex_descriptor start = vertex(0, g);

  // Compute BFS levels from vertex 0
  property_map<Graph, vertex_distance_t>::type distance =
    get(vertex_distance, g);

  put(distance, start, 0);
  breadth_first_search
    (g, start,
     visitor(make_bfs_visitor(record_distances(distance, on_tree_edge()))));

  // Output a Graphviz DOT file
  std::string outfile;

  if (argc > 2)
    outfile = argv[2];
  else {
    outfile = filename;
    int i = outfile.rfind('.');
    if (i != std::string::npos)
      outfile.erase(outfile.begin() + i, outfile.end());
    outfile += "-bfs.dot";
  }

  if (process_id(process_group(g)) == 0) {
    std::cout << "Writing GraphViz output to " << outfile << "... ";
    std::cout.flush();
  }
  write_graphviz(outfile, g,
                 make_label_writer(distance));
  if (process_id(process_group(g)) == 0)
    std::cout << "Done." << std::endl;

  return 0;
}
