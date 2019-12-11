[@react.component]
let make = () => {
  let style =
    ReactDOMRe.Style.make(~fontFamily="monospace", ~fontSize="8pt", ());
  let ((oldContent, newContent), contentSetter) =
    React.useState(() => ("", ""));
  let onChange = e => {
    let nc = e->ReactEvent.Form.target##value;
    contentSetter(((_, n)) => (n, nc));
  };

  <div>
    <p> "Playground placeholder"->React.string </p>
    <div className="input-group">
      <label className="prepend preserved-width" />
      <input style value=newContent onChange />
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
  </div>;
};
