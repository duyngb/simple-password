let onChange = (onContent, e: ReactEvent.Form.t) => {
  let value: string = e->ReactEvent.Form.target##value;
  onContent(
    value,
    String.length(value) >= 5
    && !String.contains(value, ' ')
    && !Checks.contains(value, c => Checks.isSpecialChar(c)),
  );
};

[@react.component]
let make = (~disabled, ~onContent=(_, _) => ()) => {
  <div className="input-group">
    <label className="prepend preserved-width">
      "Username"->React.string
    </label>
    <input
      type_="text"
      name="username"
      autoComplete="off"
      required=true
      placeholder="This would be permanent"
      onChange={onChange(onContent)}
      disabled
    />
  </div>;
};
