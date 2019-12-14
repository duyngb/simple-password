/**
 * Myers Diff Algorithm.
 *
 * As described in "An O(ND) Difference Algorithm and Its Variations",
 * online paper available at http://www.xmailserver.org/diff2.pdf.
 *
 * Implementation suggested by blog series [The Myers diff algorithm][1]
 * written by James Coglan.
 *
 * @module MyersDiff
 *
 * [1]: https://blog.jcoglan.com/2017/02/12/the-myers-diff-algorithm-part-1/
 */

/** Returns shorted edit graph of two list.
 *
 * As mentioned in [The Myers diff algorithm: part 2][2] by James Coglan.
 *
 * [2]: https://blog.jcoglan.com/2017/02/15/the-myers-diff-algorithm-part-2/
 */
let shortest_edit: (list(int), list(int)) => int;

/** Edit-graph type.
 *
 * Each graph/tree contains a list of node. Each node might be on of
 * `Empty` node, A `Leaf` with traversal depth, or a `Branch` with
 * conditional traversal.
 */
type editgraph =
  | Empty
  | Node(int, editgraph, editgraph);

let build_graph: (list('a), list('a), editgraph) => editgraph;

let string_of_graph: (int, editgraph) => string;
