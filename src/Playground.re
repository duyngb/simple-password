let codepointPrinter = Printf.sprintf("%x");

let differ =
    (
      updater: (string => string) => unit,
      nc /*** next content */,
      lc /*** last content */,
    ) => {
  let nr = nc |> Rope.str_to_list |> Rope.rev_utf16_be;
  let lr = lc |> Rope.str_to_list |> Rope.rev_utf16_be;

  updater(_ => nr |> List.map(codepointPrinter) |> String.concat(" "));
};

[@react.component]
let make = () => {
  let style =
    ReactDOMRe.Style.make(~fontFamily="monospace", ~fontSize="8pt", ());
  let ((oldContent, newContent), contentSetter) =
    React.useState(() => ("", ""));
  let (r, updateR) = React.useState(() => ""); // diff report

  let onChange = e => {
    let nc = e->ReactEvent.Form.target##value;
    contentSetter(x => {
      let (_, n) = x;
      // differ(updateR, nc, n);
      (n, nc);
    });
  };
  let onKeyDown = e => {
    let keyCode = e->ReactEvent.Keyboard.keyCode;
    let keyName = e->ReactEvent.Keyboard.key;
    let isShift = e->ReactEvent.Keyboard.shiftKey;
    let isCtrl = e->ReactEvent.Keyboard.ctrlKey;

    let r =
      "key code = "
      ++ keyCode->string_of_int
      ++ "\n  isShift = "
      ++ isShift->string_of_bool
      ++ "\n  isCtrl = "
      ++ isCtrl->string_of_bool
      ++ "\nFriendly key name: "
      ++ keyName;
    updateR(_ => r);

    if (keyCode == 13 && isShift) {
      contentSetter(_ => ("", ""));
    };
  };

  let onPaste = e => {
    e->ReactEvent.Clipboard.preventDefault;
    updateR(r => r ++ "\n<!> on-paste event caught and prevented!");
  };

  let onDrop = e => {
    e->ReactEvent.Mouse.preventDefault;
    updateR(r => r ++ "\n<!> on-drop event caught and prevented!");
  };

  <div>
    <p> "Playground placeholder"->React.string </p>
    <div className="input-group">
      <label className="prepend preserved-width" />
      <input
        style
        value=newContent
        spellCheck=false
        onChange
        onKeyDown
        onPaste
        onDrop
      />
    </div>
    <div className="rows">
      <div className="input-group">
        <label className="prepend preserved-width">
          "Old Content"->React.string
        </label>
        <span style> oldContent->React.string </span>
      </div>
      <div className="input-group">
        <label className="prepend preserved-width">
          "New Content"->React.string
        </label>
        <span style> newContent->React.string </span>
      </div>
    </div>
    <div className="input-group">
      <label className="prepend preserved-width">
        "Report Zone"->React.string
      </label>
      <span> <pre style> r->React.string </pre> </span>
    </div>
  </div>;
};
