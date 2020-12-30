let tail = s => String.sub(s, 1, String.length(s) - 1);

let isSpecialChar = ch =>
  switch (ch) {
  | '!'..'/' => true
  | ':'..'@' => true
  | '['..'`' => true
  | '{'..'~' => true
  | _ => false
  };

// TODO: Extends this to more exotic number forms
let isNumberEx = ch => ch >= '0' && ch <= '9';

let rec repetitiveChecker = (s, lastChar) =>
  switch (lastChar, s) {
  | (_, "") => true
  | (None, s) => repetitiveChecker(tail(s), Some(s.[0]))
  | (Some(c), s) =>
    let hd = s.[0];
    let tl = tail(s);
    c == hd ? false : repetitiveChecker(tl, Some(hd));
  };

/** Check if this string has emoji or not. */
let isHasEmoji = s =>
  s
  ->Rope.str_to_list
  ->Rope.rev_utf16_be
  ->Emo.emoji_folder
  ->List.exists(x => x != [], _);

let emoPointer = (s: string) =>
  s
  |> Rope.str_to_list
  |> Rope.rev_utf16_be
  |> Emo.emoji_folder
  |> List.exists(
       fun
       | [0x1F448 | 0x1F449]
       | [0x1F448 | 0x1F449, 0x1F3FB | 0x1F3FC | 0x1F3FD | 0x1F3FE | 0x1F3FF] =>
         true
       | _ => false,
     );

/** Check for condition defined in predicate over each character of string. */
let contains = (s: string, predicate: char => bool) =>
  s |> String.map(ch => predicate(ch) ? 'o' : 'x') |> String.contains(_, 'o');

type rule_type =
  | Normal // used for positive rule
  | ShowOnFailed; // use for negative/forbid rule

/**
 * Rule type.
 *
 * Each rule includes a checker and failed reason text.
 */
type rule = {
  c: State.t => bool, // State checker function
  r: string, // Failed reason
  d: option(State.t => State.t), // optional state reducer
  t: rule_type,
};

/** Predefined rules. */
let rules: list(rule) = [
  {
    c: _ => true,
    r: "Please fill in your identity slowly.",
    d: Some(s => {...s, disabled: true, content: ""}),
    t: ShowOnFailed,
  },
  {
    c: s => String.length(s.content) >= 8,
    r: "Password must be at least 8 characters.",
    d: None,
    t: Normal,
  },
  {
    c: s => contains(s.content, ch => ch >= 'A' && ch <= 'Z'),
    r: "Password must contain at least one common Latin uppercase letter.",
    d: None,
    t: Normal,
  },
  {
    c: s => contains(s.content, ch => ch >= '0' && ch <= '9'),
    r: "Password must contain at least one Latin number.",
    d: None,
    t: Normal,
  },
  {
    c: s => contains(s.content, isSpecialChar),
    r: "Password must contain at least one special character found on US keyboard.",
    d: None,
    t: Normal,
  },
  {
    c: s => repetitiveChecker(s.content, None),
    r: "Password must not contain repetitive pattern.",
    d: None,
    t: ShowOnFailed,
  },
  {
    c: s => String.contains(s.content, 'q'),
    r: "Password must contain character 'q' (lowercase).",
    d: None,
    t: Normal,
  },
  // New Windows update makes it extremely infuriated to enter emoji in a
  // password field. Let's try this!
  {
    c: s => isHasEmoji(s.content),
    r: "Must have one emoji.",
    d: None,
    t: Normal,
  },
  {
    c: s => emoPointer(s.content),
    r: "Must point left or right.",
    d: None,
    t: Normal,
  },
  {
    c: s => String.contains(s.content, 'x'),
    r: "Password must contain character 'x' (lowercase).",
    d: None,
    t: Normal,
  },
  {
    c: s => s.content->String.length > 0 && s.content.[0]->isNumberEx || true,
    r: "Password must not begin with a number",
    d: None,
    t: ShowOnFailed,
  },
  {
    c: s => !String.contains(s.content, ' '),
    r: "Space is not allowed in password (for an obvious security reason).",
    d: None,
    t: ShowOnFailed,
  },
  {
    c: s => String.length(s.content) <= 14,
    r: "Password must be at most 14 characters.",
    d: None,
    t: ShowOnFailed,
  },
  {
    c: st =>
      switch (st.respected) {
      | None => false
      | Some(respected) => respected
      },
    r: "Press F to pay respect for the unfortunates...",
    d: Some(s => s.respected == None ? {...s, respected: Some(false)} : s),
    t: ShowOnFailed,
  },
];
