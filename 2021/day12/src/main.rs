// graph traversal, oh boy
// distinct paths -> no adjacent big caves in input
// can probably assume no duplicate connections?
// didn't want to make another set of graph structure so imported a crate
// made the graph and then decided i wasn't really interested in the rest

use std::collections::{HashSet, HashMap};

use lib::*;
use petgraph::graph;

const SAMPLE1: &str =
"start-A
start-b
A-c
A-b
b-d
A-end
b-end";

const SAMPLE2: &str =
"dc-end
HN-start
start-kj
dc-start
dc-HN
LN-dc
HN-end
kj-sa
kj-HN
kj-dc";

fn parse_line(line: &str) -> (&str, &str) {
    let vec: Vec<&str> = line.split('-').collect();
    (vec[0], vec[1])
}

fn parse_input(inp: &str) -> (graph::NodeIndex, graph::UnGraph<&str, &str>) {
    let delim = if inp.len() <= SAMPLE2.len() {
        "\n"
    } else {
        "\r\n"
    };

    let edges: Vec<(&str, &str)> = inp.split(delim).map(parse_line).collect();
    let mut node_names = HashSet::new();
    for edge in &edges {
        node_names.insert(edge.0);
        node_names.insert(edge.1);
    }
    let mut caves = graph::Graph::new_undirected();
    let mut name_to_node_index = HashMap::new();

    node_names.remove("start");
    let start = caves.add_node("start");
    name_to_node_index.insert("start", start);

    for name in node_names {
        name_to_node_index.insert(name, caves.add_node(name));
    }
    for edge in &edges {
        caves.add_edge(
            *name_to_node_index.get(edge.0).unwrap(),
            *name_to_node_index.get(edge.1).unwrap(),
            ""
        );
    }

    (start, caves)
}

fn main() {
    let input = input_string!("input.txt");

    println!("{:?}", parse_input(SAMPLE1));

    // println!("{:?}", parse_input(&input));
}
