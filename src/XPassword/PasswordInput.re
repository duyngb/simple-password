// Internal constrains for password validation

// Unified password field state.
type state = {
  content: string,
  showed: bool,
};

let initState = {content: "", showed: false};

// Available action
type action =
  | Toggle;

let reducer = (s, action) =>
  switch (action) {
  | Toggle => {...s, showed: !s.showed}
  };

[@react.component]
let make = () => {
  let (s, dispatch) = React.useReducer(reducer, initState);

  <label>
    {React.string("Password")}
    <input
      type_={s.showed ? "string" : "password"}
      name="password"
      required=true
      minLength=8
    />
    <input
      type_="checkbox"
      name="show-text"
      checked={s.showed}
      onChange={_ => dispatch(Toggle)}
    />
  </label>;
};
