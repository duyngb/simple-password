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


ReactDOMRe.render(
  <XPassword>
  </XPassword>,
  makeContainer("New Identity"),
);