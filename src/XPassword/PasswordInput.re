// Internal constrains for password validation

type rule = {
  c: string => bool, // Checker
  r: string // Failed reason
};

let rules: list(rule) = [
  {
    c: s => String.length(s) > 8,
    r: "Password must be longer than 8 characters.",
  },
  {
    c: s => String.length(s) < 14,
    r: "Password must be shorter than 14 characters.",
  },
  {
    c: s => String.contains(s, 'Q'),
    r: "Password must contain character 'Q'.",
  },
  {
    c: s => String.contains(s, 'x'),
    r: "Password must contain character 'x'.",
  },
];

// Unified password field state.
type state = {
  content: string, // password content
  valid: bool, // is this field valid?
  passed: list(string), // passed rules
  failed: string, // current failed rule
  showed: bool // is this field currently plain text
};

let initState = {
  content: "",
  valid: false,
  passed: [],
  failed: "",
  showed: false,
};

let headStr = l =>
  switch (l) {
  | [] => ""
  | [(_, a), ..._] => a
  };

// Utilities for spliting validateMap into passes and fails
let rec ruleSplit = (a, allPassed) =>
  switch (a) {
  | [] => (allPassed, "") // All true
  | [(false, rule), ..._] => (allPassed, rule.r)
  | [(true, rule), ...rest] => ruleSplit(rest, [rule.r, ...allPassed])
  };

// Available action
type action =
  | OnUserInput(string)
  | Toggle;

let reducer = (s, action) =>
  switch (action) {
  | OnUserInput(c) =>
    let validateMap = List.map(rule => rule.c(c), rules);
    let (p, f) = ruleSplit(List.combine(validateMap, rules), []);
    {
      ...s,
      content: c,
      valid: List.for_all(i => i, validateMap),
      passed: p,
      failed: f,
    };

  | Toggle => {...s, showed: !s.showed}
  };

[@react.component]
let make = () => {
  let (s, d) = React.useReducer(reducer, initState);

  <div>
    <label>
      {React.string("Password")}
      <input
        type_={s.showed ? "string" : "password"}
        name="password"
        required=true
        onChange={e => d(OnUserInput(e->ReactEvent.Form.target##value))}
      />
      <input
        type_="checkbox"
        name="show-text"
        checked={s.showed}
        onChange={_ => d(Toggle)}
      />
      {React.string(s.valid ? "Go on! No time for waiting" : "Opp")}
    </label>
    <div>
      {List.map(
         pass => <div> {React.string("v")} {React.string(pass)} </div>,
         s.passed,
       )
       ->Array.of_list
       ->React.array}
    </div>
    <div>
      {s.failed == ""
         ? <div />
         : <div> {React.string("x")} {React.string(s.failed)} </div>}
    </div>
  </div>;
};
