// Entry point

[@bs.val] external document: Js.t({..}) = "document";

let makeContainer = text => {
  let containerRow = document##createElement("div");
  containerRow##className #= "container-row";

  let container = document##createElement("div");
  container##className #= "container";

  let title = document##createElement("div");
  title##className #= "containerTitle";
  title##innerText #= text;

  let content = document##createElement("div");
  content##className #= "containerContent";

  let main = document##getElementById("main");

  let () = container##appendChild(title);
  let () = container##appendChild(content);
  let () = containerRow##appendChild(container);
  let () = main##appendChild(containerRow);

  content;
};

ReactDOMRe.render(<Playground />, makeContainer("Playground"));

// ReactDOMRe.render(<XPassword />, makeContainer("A Simple Register Form"));

// let () = {
//   let a = ['a', 'b', 'b', 'd'];
//   let b = ['a', 'b', 'c', 'd'];
//   Js.log3("Checking", a |> Array.of_list, b |> Array.of_list);
//   Js.log(
//     MyersDiff.build_graph(a, b, MyersDiff.Empty)
//     |> MyersDiff.string_of_graph(0),
//   );
// };
