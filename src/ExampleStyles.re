let reasonReactBlue = "#48a9dc";
let borderRadius = "0.5rem";
let borderRadius2 = "0.25rem";

// The {j|...|j} feature is just string interpolation, from
// bucklescript.github.io/docs/en/interop-cheatsheet#string-unicode-interpolation
// This allows us to conveniently write CSS, together with variables, by
// constructing a string
let style = {j|
  html, body {
    width: 100vw;
    margin: 0;
  }
  body {
    background-color: rgb(224, 226, 229);
    display: flex;
    flex-direction: column;
    align-items: center;
  }
  button {
    background-color: white;
    color: $reasonReactBlue;
    box-shadow: 0 0 0 1px $reasonReactBlue;
    border: none;
    padding: 8px;
    font-size: 16px;
  }
  button:active {
    background-color: $reasonReactBlue;
    color: white;
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
    border-radius: $borderRadius;
    font-family: sans-serif;
  }
  .containerTitle {
    background-color: rgb(242, 243, 245);
    border-radius: $borderRadius $borderRadius 0px 0px;
    padding: 12px;
    font-weight: bold;
  }
  .containerContent {
    background-color: white;
    padding: 16px;
    border-radius: 0px 0px $borderRadius $borderRadius;
  }
@media screen and (min-width: 400px) {
  html, body { overflow-x: hidden; }
}
|j};
