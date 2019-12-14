// Let's branching with the above implementation!

type editgraph =
  | Empty
  | Node(int, editgraph, editgraph);

let f = g =>
  switch (g) {
  | Empty => Empty
  | _ => g
  };

let rec build_graph = (a, b, root) =>
  switch (a, b) {
  | ([], []) => root
  | (a, []) =>
    // New list exhausted. Remains are deletions.
    Js.log("New list exhausted. Remains are all deletions.");
    switch (root) {
    | Empty => Node(0, Node(a->List.length, Empty, Empty), Empty)
    | Node(x, _d, i) => Node(x, Node(a->List.length, Empty, Empty), i)
    };
  | ([], b) =>
    // Old list exhausted. Remains are insertions.
    Js.log("Old list exhausted. Remains are all insertions.");
    switch (root) {
    | Empty => Node(0, Empty, Node(b->List.length, Empty, Empty))
    | Node(x, d, _i) => Node(x, d, Node(b->List.length, Empty, Empty))
    };
  | ([a1, ...a2], [b1, ...b2]) when a1 == b1 =>
    switch (root) {
    | Empty => build_graph(a2, b2, Node(1, Empty, Empty))
    | Node(x, d, i) => build_graph(a2, b2, Node(x + 1, d, i))
    }
  | ([_, ...a2], [_, ...b2]) =>
    switch (root) {
    | Empty =>
      Node(
        0,
        build_graph(a2, b, Node(0, Empty, Empty)) |> f /* Assume deletion op */,
        build_graph(a, b2, Node(0, Empty, Empty)) |> f /* Assume insertion op */,
      )
    | Node(x, d, i) =>
      Node(
        x,
        build_graph(a2, b, d) |> f /* */,
        build_graph(a, b2, i) |> f /* */,
      )
    }
  };

let rec string_of_graph = (indent, graph) => {
  let i = String.make(indent, ' ');
  switch (graph) {
  | Empty => i ++ "<empty>"
  | Node(n, Empty, Empty) => i ++ {j|Leaf(x=$n)|j}
  | Node(n, l, r) =>
    let s1 = string_of_graph(indent + 4, l);
    let s2 = string_of_graph(indent + 4, r);
    i ++ {j|Node(x=$n)
$s1
$s2|j};
  };
};

let shortest_edit = (_a, _b) => {
  0;
};
