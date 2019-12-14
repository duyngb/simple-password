let codepointPrinter = Printf.sprintf("%x");

let differ =
    (
      updater: (string => string) => unit,
      lc /*** last content */,
      nc /*** next content */,
    ) => {
  let nr = nc |> Rope.str_to_list |> Rope.rev_utf16_be;
  let lr = lc |> Rope.str_to_list |> Rope.rev_utf16_be;

  // let n = nr |> List.map(codepointPrinter) |> String.concat(" ");
  // let l = lr |> List.map(codepointPrinter) |> String.concat(" ");

  let se =
    MyersDiff.build_graph(lr, nr, MyersDiff.Empty)
    |> MyersDiff.string_of_graph(0);

  updater(_ => se);
};

[@react.component]
let make = () => {
  let style =
    ReactDOMRe.Style.make(~fontFamily="monospace", ~fontSize="8pt", ());
  let ((a, b), contentSetter) = React.useState(() => ("ABCABBA", "CBABAC"));
  let (r, updateR) =
    React.useState(() => {
      let lr = a |> Rope.str_to_list |> Rope.rev_utf16_be;
      let nr = b |> Rope.str_to_list |> Rope.rev_utf16_be;

      MyersDiff.build_graph(lr, nr, MyersDiff.Empty)
      |> MyersDiff.string_of_graph(0);
    }); // diff report

  <>
    <div className="rows">
      <div className="input-group">
        <label className="prepend preserved-width">
          "Old Content"->React.string
        </label>
        <input
          style
          value=a
          onChange={e => {
            let c = e->ReactEvent.Form.target##value;
            contentSetter(((_, n)) => {
              differ(updateR, c, n);
              (c, n);
            });
          }}
        />
      </div>
      <div className="input-group">
        <label className="prepend preserved-width">
          "New Content"->React.string
        </label>
        <input
          style
          value=b
          onChange={e => {
            let c = e->ReactEvent.Form.target##value;
            contentSetter(((o, _)) => {
              differ(updateR, o, c);
              (o, c);
            });
          }}
        />
      </div>
    </div>
    <div className="input-group">
      <label className="prepend preserved-width">
        "Report Zone"->React.string
      </label>
      <span> <pre style> r->React.string </pre> </span>
      <button className="button append" onClick={_ => differ(updateR, a, b)}>
        "Update"->React.string
      </button>
    </div>
  </>;
};
