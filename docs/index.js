(self.webpackChunkreasonml_password_challenge=self.webpackChunkreasonml_password_challenge||[]).push([[826],{987:(e,t,n)=>{"use strict";var r=n(294),a=n(935),i=n(133);a.render(r.createElement(i.make,{}),function(e){var t=document.createElement("div");t.className="container-row";var n=document.createElement("div");n.className="container";var r=document.createElement("div");r.className="containerTitle",r.innerText=e;var a=document.createElement("div");a.className="containerContent";var i=document.getElementById("main");return n.appendChild(r),n.appendChild(a),t.appendChild(n),i.appendChild(t),a}("New Identity"))},820:(e,t,n)=>{"use strict";var r=n(640),a=n(562),i=n(574),o=n(300),s=n(294),c=n(331),u=n(849),d=n(694),l=n(867),f=n(960);function m(e){return c.sub(e,1,e.length-1|0)}function p(e,t){return a.of_list(r.mapi((function(t,n){return o._2(e,String(t),n)}),t))}function v(e){return e>=65?e>=97?!(e>126||e<123):e>=91:e>=48?e>=58:e>=33}function h(e,t){for(;;){var n=t,r=e;if(""===r)return!0;if(void 0===n)t=d.get(r,0),e=m(r);else{var a=d.get(r,0),i=m(r);if(n===a)return!1;t=a,e=i}}}function b(e){return r.exists((function(e){return 0!==e}),l.emoji_folder(f.rev_utf16_be(f.str_to_list(e))))}function _(e){return r.exists((function(e){if(!e)return!1;var t=e[0];if(t<128072)return!1;if(t>=128074)return!1;var n=e[1];if(!n)return!0;var r=n[0];return r>=127995&&!(r>=128e3||n[1])}),l.emoji_folder(f.rev_utf16_be(f.str_to_list(e))))}var w={content:"",respected:void 0,passed:0,failed:void 0,showed:!1,iteration:0,disabled:!1,timer:!1};function E(e,t){return c.contains(c.map((function(e){return o._1(t,e)?111:120}),e),111)}var g=[{c:function(e){return e.content.length>=8},r:"Password must be at least 8 characters.",d:void 0,t:0},[{c:function(e){return E(e.content,(function(e){return e>=65&&e<=90}))},r:"Password must contain at least one common Latin uppercase letter.",d:void 0,t:0},[{c:function(e){return E(e.content,(function(e){return e>=48&&e<=57}))},r:"Password must contain at least one Latin number.",d:void 0,t:0},[{c:function(e){return E(e.content,v)},r:"Password must contain at least one special character found on US keyboard.",d:void 0,t:0},[{c:function(e){return h(e.content,void 0)},r:"Password must not contain repetitive pattern.",d:void 0,t:0},[{c:function(e){return c.contains(e.content,113)},r:"Password must contain character 'q' (lowercase).",d:void 0,t:0},[{c:function(e){return c.contains(e.content,120)},r:"Password must contain character 'x' (lowercase).",d:void 0,t:0},[{c:function(e){var t=e.respected;return void 0!==t&&t},r:"Press F to pay respect!",d:function(e){return void 0===e.respected?{content:e.content,respected:!1,passed:e.passed,failed:e.failed,showed:e.showed,iteration:e.iteration,disabled:e.disabled,timer:e.timer}:e},t:1},[{c:function(e){return b(e.content)},r:"Must have one emoji.",d:void 0,t:0},[{c:function(e){return _(e.content)},r:"Must point left or right.",d:void 0,t:0},[{c:function(e){return!c.contains(e.content,32)},r:"Space is not allowed in password (for an obvious security reason).",d:void 0,t:1},[{c:function(e){return e.content.length<=14},r:"Password must be at most 14 characters.",d:void 0,t:0},0]]]]]]]]]]]];function k(e,t){for(;;){var n=t,r=e;if(!r)return[n,void 0];var a=r[0];if(!a[0])return[n,a[1]];var i=r[1],o=a[1];o.t||(t=[o.r,n]),e=i}}function N(e){return k(r.combine(r.map((function(t){return o._1(t.c,e)}),g),g),0)}function C(e,t,n){var r=e.respected;if(void 0!==r&&!r)return n.preventDefault(),o._1(t,i.__(1,[n.keyCode]))}function y(e,t){var n=N({content:t,respected:e.respected,passed:e.passed,failed:e.failed,showed:e.showed,iteration:e.iteration,disabled:e.disabled,timer:e.timer}),r=n[1],a=n[0],i=u.caml_equal(e.respected,!1)?e.content:t,s=e.respected,c=(e.failed,e.showed),d=e.iteration,l=e.disabled,f=e.timer;if(void 0===r)return{content:i,respected:s,passed:a,failed:void 0,showed:c,iteration:d,disabled:l,timer:f};var m=u.caml_notequal(r.r,e.failed)?e.iteration+1|0:e.iteration,p=r.d;return void 0!==p?o._1(p,{content:i,respected:s,passed:a,failed:r.r,showed:c,iteration:m,disabled:l,timer:f}):{content:i,respected:s,passed:a,failed:r.r,showed:c,iteration:m,disabled:l,timer:f}}function S(e,t){if("number"==typeof t)return 0===t?w:{content:e.content,respected:e.respected,passed:e.passed,failed:e.failed,showed:!e.showed,iteration:e.iteration,disabled:e.disabled,timer:!0};switch(0|t.tag){case 0:return y(e,t[0]);case 1:return y({content:e.content,respected:70===t[0],passed:e.passed,failed:e.failed,showed:e.showed,iteration:e.iteration,disabled:e.disabled,timer:e.timer},e.content);case 2:var n=t[0];return setTimeout((function(e){return o._1(n,0)}),5e3),{content:e.content,respected:void 0,passed:0,failed:void 0,showed:!1,iteration:0,disabled:!0,timer:!1}}}function P(e){var t=e.fs,n=e.endProgressCb,r=void 0!==n?n:function(e){};return s.createElement("div",{className:"progress"},s.createElement("div",{className:"progress-bar",disabled:!t,onAnimationEnd:r}))}var x={make:P};t.tail=m,t.fxi=p,t.isSpecialChar=v,t.repetitiveChecker=h,t.isHasEmoji=b,t.emoPointer=_,t.initState=w,t.isStateValid=function(e){return!!u.caml_equal(e.respected,!0)&&void 0===e.failed},t.strCheck=E,t.rules=g,t.ruleSplit=k,t.ruleReasons=function(e){return r.map((function(e){return e.r}),e)},t.findFailed=N,t.keydownHandler=C,t.stateCheck=y,t.reducer=S,t.onProgressEnd=function(e,t,n){return o._1(t,(function(e){return!1})),o._1(e,0)},t.enableTimer=function(e,t,n){return o._1(t,(function(e){return!0})),o._1(e,1)},t.Timer=x,t.make=function(e){var t,n=e.passSetter,r=s.useReducer(S,w),a=r[1],c=r[0],u=s.useState((function(){return!1})),d=u[1];if(0===c.iteration)t=null;else{var l=c.failed;t=s.createElement("div",{className:"input-group"},s.createElement("div",{className:"reasons"},void 0!==l?s.createElement("div",{key:String(c.iteration),className:"failed"},l):(o._1(n,(function(e){return!0})),s.createElement("p",void 0,"You are good to go!")),p((function(e,t){return s.createElement("div",{key:e,className:"passed"},t)}),c.passed)))}return s.createElement(s.Fragment,void 0,s.createElement("div",{className:"input-group"},s.createElement("label",{className:"prepend preserved-width"},"Password"),s.createElement("input",{autoComplete:"off",disabled:c.disabled,maxLength:25,minLength:8,name:"password",placeholder:c.disabled?"You...?":"Just a simple password...",required:!0,type:c.showed?"text":"password",value:c.content,onPaste:function(e){return o._1(a,i.__(2,[a]))},onKeyDown:function(e){return C(c,a,e)},onChange:function(e){return o._1(a,i.__(0,[e.target.value]))}}),0===c.iteration?null:s.createElement("button",{className:"append button",reversed:c.showed,onClick:function(e){return o._1(d,(function(e){return!0})),o._1(a,1)}},s.createElement("i",void 0,"Hint"))),c.timer?s.createElement("div",{className:"input-group"},s.createElement("div",{className:"prepend preserved-width"}),s.createElement(P,{fs:u[0],endProgressCb:function(e){return o._1(d,(function(e){return!1})),o._1(a,0)}})):null,t)}},133:(e,t,n)=>{"use strict";var r=n(300),a=n(294),i=n(331),o=n(820);function s(e,t){var n=t.target.value,a=n.length>=5&&!i.contains(n,32)&&o.strCheck(n,(function(e){return!o.isSpecialChar(e)}));return r._1(e,(function(e){return a}))}function c(e){var t=e.disabled,n=e.userSetter;return a.createElement("div",{className:"input-group"},a.createElement("label",{className:"prepend preserved-width"},"Username"),a.createElement("input",{autoComplete:"off",disabled:t,name:"username",placeholder:"This would be permanent",required:!0,type:"text",onChange:function(e){return s(n,e)}}))}var u={onChange:s,make:c};t.CompUsername=u,t.freezeUsrField=function(e,t,n){return r._1(e,(function(e){return!1})),r._1(t,(function(e){return!0}))},t.make=function(e){var t,n,i=a.useState((function(){return 0})),s=i[1],u=i[0],d=a.useState((function(){return!1})),l=a.useState((function(){return!1}));switch(u){case 0:t="Please choose a name for your eternity";break;case 1:t="Just another simple field and you are done!";break;case 2:t="That is. What a day!"}switch(u){case 0:n=a.createElement("div",{className:"input-group"},a.createElement("button",{className:"button",disabled:!d[0],onClick:function(e){return r._1(s,(function(e){return 1}))}},"I accept the risk"));break;case 1:n=a.createElement("div",{className:"input-group"},a.createElement("button",{className:"button",disabled:!l[0],type:"submit",onClick:function(e){return r._1(s,(function(e){return 2}))}},"I'm done"));break;case 2:n=null}return a.createElement("div",{className:"reg-form"},a.createElement("p",void 0,t),a.createElement("div",{className:"rows"},a.createElement(c,{disabled:0!==u,userSetter:d[1]}),u>=1?a.createElement(o.make,{passSetter:l[1]}):null),a.createElement("div",{className:"rows"},n),a.createElement("p",{className:"note"},a.createElement("i",void 0,"All fields are required")))}},867:(e,t,n)=>{"use strict";var r=n(640);function a(e,t){if(t.state)return{acc:[[e,t.buff],t.acc],buff:0,state:0};if(e>=65038){if(e>=127995){if(e<128e3)return{acc:t.acc,buff:[e,0],state:1}}else if(e<65040)return{acc:t.acc,buff:[e,0],state:1}}else if(8205===e){var n=t.acc,r=n?[n[0],n[1]]:[0,0];return{acc:r[1],buff:[e,r[0]],state:1}}return e>=127536&&e<129696?{acc:[[e,0],t.acc],buff:t.buff,state:t.state}:t}t.emoji_folder=function(e){return r.fold_right(a,e,{acc:0,buff:0,state:0}).acc}},960:(e,t,n)=>{"use strict";var r=n(640),a=n(562),i=n(331),o=n(231),s=n(813);function c(e,t,n){return 0!==e?e<=55295?[e,n]:e<=57343?n:e<=65535?[e,n]:e<=1114111?[e>>>10&1023|55296,[1023&e|56320,n]]:n:n}function u(e,t,n){return 0!==e?e<=55295?[e,n]:e<=56319?[65536|1023&t|(1023&e)<<10,n]:e<=57343?n:[e,n]:n}t.str_to_list=function(e){var t=o.caml_make_vect(e.length,0);return i.iteri((function(e,n){return o.caml_array_set(t,e,n)}),e),a.to_list(t)},t.rev_utf16_be=function(e){return r.fold_right2(u,s.$at([0,0],e),s.$at(e,[0,0]),0)},t.fwd_utf16_be=function(e){return r.fold_right2(c,s.$at([0,0],e),s.$at(e,[0,0]),0)}}},0,[[987,666,216]]]);