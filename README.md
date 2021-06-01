# Simple password playground

Based on a battle posted [here] and [there]!

This repository is an experiment on [ReasonML].

## Try out?

Install and run, as a normal Node project:

```sh
npm install

# This is for one-time bsb build:
npx bsb -make-world

npm run start:dev
```

Then head to http://127.0.0.1:9000 in your browser.

Usually, your IDE/Reason language server should watch and build `*.re` files
for you. If this is not the case, run watcher in another shell:

```sh
# This should watch for re file changes and rebuild
npm run watch
```

Webpack dev server will catch those `*.js` changes and update its bundle
content. A manual reload in browser is required in the meantime.

[here]: https://www.reddit.com/r/badUIbattles/comments/e4thbk "r/badUIbattles"
[there]: https://twitter.com/notdetails/status/1201015962398539777 "@notdetails"
[ReasonML]: https://reasonml.github.io "ReasonML"
