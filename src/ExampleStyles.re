let reasonReactBlue = "#48a9dc";
let r5 = "0.5rem";
let r3 = "0.375rem";
let fontFamily = "sans-serif";
let fontSize = "10pt";

// The {j|...|j} feature is just string interpolation, from
// bucklescript.github.io/docs/en/interop-cheatsheet#string-unicode-interpolation
// This allows us to conveniently write CSS, together with variables, by
// constructing a string
let style = {j|
html, body {
  width: 100vw; margin: 0;
  font-family: $fontFamily;
  font-size: $fontSize;
}

button {font-family: $fontFamily; font-size: $fontSize;}

body {
  background-color: rgb(224, 226, 229);
  display: flex;
  flex-direction: column;
  align-items: center;
}
.container-row {
  display: flex;
  flex-direction: column;
  width: 100%;
  min-width: 400px;
}
.container {
  margin: 12px;
  box-shadow: 0px 4px 16px rgb(200, 200, 200);
  border-radius: $r5;
}
.containerTitle {
  background-color: rgb(242, 243, 245);
  border-radius: $r5 $r5 0px 0px;
  padding: 12px;
  font-weight: bold;
}
.containerContent {
  background-color: white;
  padding: 16px;
  border-radius: 0px 0px $r5 $r5;
}

input { padding: 0.25rem; }

.input-group {
  display: flex;
  align-content: center;
  border-radius: 0.325rem;
  margin-bottom: 0.5rem;
}
.input-group * { transition-duration: 0.2s; }
.input-group > * {
  flex-grow: 1;
  padding: 0.375rem 0.75rem;
  margin-left: -1px;
  border: 1px solid gray;
  border-radius: $r3;
}
.input-group > .preserved-width { min-width: 5em; }
.input-group > .append, .input-group > .prepend { flex-shrink: 1; flex-grow: 0; }

.input-group > *:not(:first-child) { border-top-left-radius: 0; border-bottom-left-radius: 0;}
.input-group > *:not(:last-child) { border-top-right-radius: 0; border-bottom-right-radius: 0;}

.input-group > input:focus {
  border-color: #80bdff;
  box-shadow: 0 0 0 .2rem rgba(0,123,255,.35);
  z-index: 3;
}
.button:hover, .button[reversed] {background-color: gray; color: whitesmoke;}
.button[reversed]:hover {background-color: unset; color: unset;}

.reasons { display: flex; flex-direction: column; font-size: 10pt; }
.reasons .failed { margin: 0.15rem; color: red; animation: pushed-left 0.5s ease-in-out; }
.reasons .passed { margin: 0.15rem; color: green; }

@keyframes pushed-left {
  from { transform: translate(2rem, 0); opacity: 0; }
  to   { transform: translate(0, 0); opacity: 1; }
}

@media screen and (min-width: 400px) {
  html, body { overflow-x: hidden; }
}
|j};
