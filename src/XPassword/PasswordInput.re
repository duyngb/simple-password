let tail = s => String.sub(s, 1, String.length(s) - 1);

let fxi = (fx, list) =>
  list
  |> List.mapi((idx, a) => fx(string_of_int(idx), a))
  |> Array.of_list
  |> React.array;

let isSpecialChar = ch =>
  switch (ch) {
  | '!'..'/' => true
  | ':'..'@' => true
  | '['..'`' => true
  | '{'..'~' => true
  | _ => false
  };

let rec repetitiveChecker = (lastChar, s) =>
  switch (lastChar, s) {
  | (_, "") => true
  | (None, s) => repetitiveChecker(Some(s.[0]), tail(s))
  | (Some(c), s) =>
    let hd = s.[0];
    let tl = tail(s);
    c == hd ? false : repetitiveChecker(Some(hd), tl);
  };

/** Check for condition defined in predicate over each character of string. */
let strCheck = (predicate: char => bool, s: string) =>
  s |> String.map(ch => predicate(ch) ? 'o' : 'x') |> String.contains(_, 'o');

/** Check if this string has emoji or not. */
let isHasEmoji = s =>
  s
  |> Rope.str_to_list
  |> Rope.rev_utf16_be
  |> Emo.emoji_folder
  |> List.exists(x => x != []);

/** Unified password field state. */
type state = {
  content: string, // password content
  respected: option(bool), // Did someone respected? If None, do not check for keydown
  passed: list(string), // passed rules
  failed: option(string), // current failed rule
  showed: bool // is this field currently plain text
};

let initState = {
  content: "",
  passed: [],
  failed: None,
  showed: true,
  respected: None,
};

let isStateValid = s => s.respected == Some(true) && s.failed == None;

/**
 * Rule type.
 *
 * Each rule includes a checker and failed reason text.
 */
type rule = {
  c: state => bool, // State checker function
  r: string // Failed reason
};

let ($) = (f, g, x) => f(x) |> g;
let stc = s => s.content;

/** Predefined rules. */
let rules: list(rule) = [
  {
    c: s => String.length(s.content) > 8,
    r: "Password must be longer than 8 characters.",
  },
  {
    c: s => strCheck(ch => ch == Char.uppercase_ascii(ch), s.content),
    r: "Password must contain at least one Latin uppercase letter.",
  },
  {
    c: stc $ strCheck(ch => ch >= '0' && ch <= '9'),
    r: "Password must contain at least one Latin number.",
  },
  {
    c: stc $ strCheck(isSpecialChar),
    r: "Password must contain at least one special character found on US keyboard.",
  },
  {
    c: stc $ repetitiveChecker(None),
    r: "Password must not contain repetitive pattern.",
  },
  {
    c: st =>
      switch (st.respected) {
      | None => false
      | Some(respected) => respected
      },
    r: "Press F to pay respect!",
  },
  {
    c: stc $ String.contains(_, 'q'),
    r: "Password must contain character 'q' (lowercase).",
  },
  {
    c: stc $ String.contains(_, 'x'),
    r: "Password must contain character 'x' (lowercase).",
  },
  {
    c: stc $ isHasEmoji, //
    r: "Must have one emoji.",
  },
  {
    c: s => String.length(s.content) < 14,
    r: "Password must be shorter than 14 characters.",
  },
];

// Utilities for spliting validateMap into passes and fails
let rec ruleSplit = (a, allPassed) =>
  switch (a) {
  | [] => (allPassed, None) // All true
  | [(false, rule), ..._] => (allPassed, Some(rule.r))
  | [(true, rule), ...rest] => ruleSplit(rest, [rule.r, ...allPassed])
  };

/** Available actions. */
type action =
  | OnChange(string)
  | Respect(int)
  | Toggle;

/** Unified reducer. */
let reducer = (s, action) =>
  switch (action) {
  | OnChange(content) =>
    let validated = List.map(rule => rule.c({...s, content}), rules);
    let (passed, failed) = List.combine(validated, rules)->ruleSplit @@ [];
    {...s, content, passed, failed};
  | Respect(keyCode) => {...s, respected: Some(keyCode == 70)}
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
        minLength=8
        onChange={e => d(OnChange(e->ReactEvent.Form.target##value))}
        onKeyDown={
          switch (s.respected) {
          | Some(true) => (_ => ())
          | Some(false)
          | None => (e => e->ReactEvent.Keyboard.keyCode->Respect->d)
          }
        }
      />
      <input
        type_="checkbox"
        name="show-text"
        checked={s.showed}
        onChange={_ => d(Toggle)}
      />
      {React.string(isStateValid(s) ? "You are good to go" : "Opp")}
    </label>
    <div>
      {switch (s.failed) {
       | None => <div />
       | Some(reason) => <div> {React.string("[x] " ++ reason)} </div>
       }}
      {s.passed |> fxi((key, pass) => <div key> {React.string(pass)} </div>)}
    </div>
  </div>;
};
