/** Unified password field state. */
type t = {
  content: string, // password content
  respected: option(bool), // If None, do not check for keydown
  passed: list(string), // passed rules' text
  failed: option(string), // current failed rule
  showed: bool, // is this field currently plain text
  iteration: int, // used to assign unique index for each error message, prevent unnecessary repaint
  disabled: bool, // For some special circumstance, this input field is disabled
  timer: bool,
};
