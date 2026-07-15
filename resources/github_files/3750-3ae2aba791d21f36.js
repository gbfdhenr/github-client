performance.mark("js-parse-end:3750-3ae2aba791d21f36.js");
"use strict";(globalThis.rspackChunk_github_ui_github_ui=globalThis.rspackChunk_github_ui_github_ui||[]).push([[3750],{911117(e,t,o){var n=o(506986),r={},i={},a=new WeakMap,l=new WeakMap,u=new WeakMap,p=Object.getOwnPropertyDescriptor(Event.prototype,"currentTarget");function s(e,t,o){var n=e[t];return e[t]=function(){return o.apply(e,arguments),n.apply(e,arguments)},e}function c(){a.set(this,!0)}function f(){a.set(this,!0),l.set(this,!0)}function d(){return u.get(this)||null}function h(e,t){p&&Object.defineProperty(e,"currentTarget",{configurable:!0,enumerable:!0,get:t||p.get})}function g(e){if(function(e){try{return e.eventPhase,!0}catch(e){return!1}}(e)){var t=(1===e.eventPhase?i:r)[e.type];if(t){var o=function(e,t,o){var n=[],r=t;do{if(1!==r.nodeType)break;var i=e.matches(r);if(i.length){var a={node:r,observers:i};o?n.unshift(a):n.push(a)}}while(r=r.parentElement)return n}(t,e.target,1===e.eventPhase);if(o.length){s(e,"stopPropagation",c),s(e,"stopImmediatePropagation",f),h(e,d);for(var n=0,p=o.length;n<p&&!a.get(e);n++){var g=o[n];u.set(e,g.node);for(var v=0,m=g.observers.length;v<m&&!l.get(e);v++)g.observers[v].data.call(g.node,e)}u.delete(e),h(e)}}}}function v(e,t,o){var a=arguments.length>3&&void 0!==arguments[3]?arguments[3]:{},l=!!a.capture,u=l?i:r,p=u[e];p||(p=new n.A,u[e]=p,document.addEventListener(e,g,l)),p.add(t,o)}function m(e,t,o){return e.dispatchEvent(new CustomEvent(t,{bubbles:!0,cancelable:!0,detail:o}))}o.d(t,{h:()=>m,on:()=>v})},905225(e,t,o){function n(...e){return JSON.stringify(e,(e,t)=>"object"==typeof t?t:String(t))}function r(e,t={}){let{hash:o=n,cache:i=new Map}=t;return function(...t){let n=o.apply(this,t);if(i.has(n))return i.get(n);let r=e.apply(this,t);return r instanceof Promise&&(r=r.catch(e=>{throw i.delete(n),e})),i.set(n,r),r}}o.d(t,{A:()=>r,G:()=>n})},200913(e,t,o){o.r(t);var n=class extends Event{oldState;newState;constructor(e,{oldState:t="",newState:o="",...n}={}){super(e,n),this.oldState=String(t||""),this.newState=String(o||"")}},r=new WeakMap;function i(e,t,o){r.set(e,setTimeout(()=>{r.has(e)&&e.dispatchEvent(new n("toggle",{cancelable:!1,oldState:t,newState:o}))},0))}var a=globalThis.ShadowRoot||function(){},l=globalThis.HTMLDialogElement||function(){},u=new WeakMap,p=new WeakMap,s=new WeakMap;function c(e){return s.get(e)||"hidden"}var f=new WeakMap;function d(e,t){return!("auto"!==e.popover&&"manual"!==e.popover||!e.isConnected||t&&"showing"!==c(e)||!t&&"hidden"!==c(e)||e instanceof l&&e.hasAttribute("open"))&&document.fullscreenElement!==e}function h(e){return e?Array.from(p.get(e.ownerDocument)||[]).indexOf(e)+1:0}function g(e){let t=p.get(e);for(let e of t||[])if(e.isConnected)return e;else t.delete(e);return null}function v(e){return"function"==typeof e.getRootNode?e.getRootNode():e.parentNode?v(e.parentNode):e}function m(e){for(;e;){if(e instanceof HTMLElement&&"auto"===e.popover&&"showing"===s.get(e))return e;if((e=e instanceof Element&&e.assignedSlot||e.parentElement||v(e))instanceof a&&(e=e.host),e instanceof Document)return}}var b=new WeakMap;function w(e){if(!d(e,!1))return;let t=e.ownerDocument;if(!e.dispatchEvent(new n("beforetoggle",{cancelable:!0,oldState:"closed",newState:"open"}))||!d(e,!1))return;let o=!1;if("auto"===e.popover){let o=e.getAttribute("popover");if(T(function(e){let t=new Map,o=0;for(let n of p.get(e.ownerDocument)||[])t.set(n,o),o+=1;t.set(e,o),o+=1;let n=null;return!function(e){let o=m(e);if(null===o)return;let r=t.get(o);(null===n||t.get(n)<r)&&(n=o)}(e.parentElement||v(e)),n}(e)||t,!1,!0),o!==e.getAttribute("popover")||!d(e,!1))return}g(t)||(o=!0),b.delete(e);let r=t.activeElement;e.classList.add(":popover-open"),s.set(e,"showing"),u.has(t)||u.set(t,new Set),u.get(t).add(e),(function(e){if(e.shadowRoot&&!0!==e.shadowRoot.delegatesFocus)return null;let t=e;t.shadowRoot&&(t=t.shadowRoot);let o=t.querySelector("[autofocus]");if(o)return o;for(let e of t.querySelectorAll("slot"))for(let t of e.assignedElements({flatten:!0}))if(t.hasAttribute("autofocus"))return t;else if(o=t.querySelector("[autofocus]"))return o;let n=e.ownerDocument.createTreeWalker(t,NodeFilter.SHOW_ELEMENT),r=n.currentNode;for(;r;){var i;if(!((i=r).hidden||i instanceof a||(i instanceof HTMLButtonElement||i instanceof HTMLInputElement||i instanceof HTMLSelectElement||i instanceof HTMLTextAreaElement||i instanceof HTMLOptGroupElement||i instanceof HTMLOptionElement||i instanceof HTMLFieldSetElement)&&i.disabled||i instanceof HTMLInputElement&&"hidden"===i.type||i instanceof HTMLAnchorElement&&""===i.href)&&"number"==typeof i.tabIndex&&-1!==i.tabIndex)return r;r=n.nextNode()}})(e)?.focus(),"auto"===e.popover&&(p.has(t)||p.set(t,new Set),p.get(t).add(e),A(f.get(e),!0)),o&&r&&"auto"===e.popover&&b.set(e,r),i(e,"closed","open")}function y(e,t=!1,o=!1){if(!d(e,!0))return;let r=e.ownerDocument;if("auto"===e.popover&&(T(e,t,o),!d(e,!0))||(A(f.get(e),!1),f.delete(e),o&&(e.dispatchEvent(new n("beforetoggle",{oldState:"open",newState:"closed"})),!d(e,!0))))return;u.get(r)?.delete(e),p.get(r)?.delete(e),e.classList.remove(":popover-open"),s.set(e,"hidden"),o&&i(e,"open","closed");let a=b.get(e);a&&(b.delete(e),t&&a.focus())}function E(e,t=!1,o=!1){let n=g(e);for(;n;)y(n,t,o),n=g(e)}function T(e,t,o){let n=e.ownerDocument||e;if(e instanceof Document)return E(n,t,o);let r=null,i=!1;for(let t of p.get(n)||[])if(t===e)i=!0;else if(i){r=t;break}if(!i)return E(n,t,o);for(;r&&"showing"===c(r)&&p.get(n)?.size;)y(r,t,o)}var S=new WeakMap;function M(e){let t,o;if(!e.isTrusted)return;let n=e.composedPath()[0];if(!n)return;let r=n.ownerDocument;if(!g(r))return;let i=(t=m(n),o=function(e){for(;e;){let t=e.popoverTargetElement;if(t instanceof HTMLElement)return t;if((e=e.parentElement||v(e))instanceof a&&(e=e.host),e instanceof Document)return}}(n),h(t)>h(o)?t:o);if(i&&"pointerdown"===e.type)S.set(r,i);else if("pointerup"===e.type){let e=S.get(r)===i;S.delete(r),e&&T(i||r,!1,!0)}}var L=new WeakMap;function A(e,t=!1){if(!e)return;L.has(e)||L.set(e,e.getAttribute("aria-expanded"));let o=e.popoverTargetElement;if(o instanceof HTMLElement&&"auto"===o.popover)e.setAttribute("aria-expanded",String(t));else{let t=L.get(e);t?e.setAttribute("aria-expanded",t):e.removeAttribute("aria-expanded")}}var k=globalThis.ShadowRoot||function(){};function H(){return"u">typeof HTMLElement&&"object"==typeof HTMLElement.prototype&&"popover"in HTMLElement.prototype}function P(){return!!(document.body?.showPopover&&!/native code/i.test(document.body.showPopover.toString()))}function D(e,t,o){let n=e[t];Object.defineProperty(e,t,{value(e){return n.call(this,o(e))}})}var x=/(^|[^\\]):popover-open\b/g,W=null;function O(e){let t,o=(t="function"==typeof globalThis.CSSLayerBlockRule,`
${t?"@layer popover-polyfill {":""}
  :where([popover]) {
    position: fixed;
    z-index: 2147483647;
    inset: 0;
    padding: 0.25em;
    width: fit-content;
    height: fit-content;
    border-width: initial;
    border-color: initial;
    border-image: initial;
    border-style: solid;
    background-color: canvas;
    color: canvastext;
    overflow: auto;
    margin: auto;
  }

  :where([popover]:not(.\\:popover-open)) {
    display: none;
  }

  :where(dialog[popover].\\:popover-open) {
    display: block;
  }

  :where(dialog[popover][open]) {
    display: revert;
  }

  :where([anchor].\\:popover-open) {
    inset: auto;
  }

  :where([anchor]:popover-open) {
    inset: auto;
  }

  @supports not (background-color: canvas) {
    :where([popover]) {
      background-color: white;
      color: black;
    }
  }

  @supports (width: -moz-fit-content) {
    :where([popover]) {
      width: -moz-fit-content;
      height: -moz-fit-content;
    }
  }

  @supports not (inset: 0) {
    :where([popover]) {
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
    }
  }
${t?"}":""}
`);if(null===W)try{(W=new CSSStyleSheet).replaceSync(o)}catch{W=!1}if(!1===W){let t=document.createElement("style");t.textContent=o,e instanceof Document?e.head.prepend(t):e.prepend(t)}else e.adoptedStyleSheets=[W,...e.adoptedStyleSheets]}function C(){var e;if("u"<typeof window)return;function t(e){return e?.includes(":popover-open")&&(e=e.replace(x,"$1.\\:popover-open")),e}window.ToggleEvent=window.ToggleEvent||n,D(Document.prototype,"querySelector",t),D(Document.prototype,"querySelectorAll",t),D(Element.prototype,"querySelector",t),D(Element.prototype,"querySelectorAll",t),D(Element.prototype,"matches",t),D(Element.prototype,"closest",t),D(DocumentFragment.prototype,"querySelectorAll",t),Object.defineProperties(HTMLElement.prototype,{popover:{enumerable:!0,configurable:!0,get(){if(!this.hasAttribute("popover"))return null;let e=(this.getAttribute("popover")||"").toLowerCase();return""===e||"auto"==e?"auto":"manual"},set(e){null===e?this.removeAttribute("popover"):this.setAttribute("popover",e)}},showPopover:{enumerable:!0,configurable:!0,value(){w(this)}},hidePopover:{enumerable:!0,configurable:!0,value(){y(this,!0,!0)}},togglePopover:{enumerable:!0,configurable:!0,value(e){"showing"===s.get(this)&&void 0===e||!1===e?y(this,!0,!0):(void 0===e||!0===e)&&w(this)}}});let o=Element.prototype.attachShadow;o&&Object.defineProperties(Element.prototype,{attachShadow:{enumerable:!0,configurable:!0,writable:!0,value(e){let t=o.call(this,e);return O(t),t}}});let r=HTMLElement.prototype.attachInternals;r&&Object.defineProperties(HTMLElement.prototype,{attachInternals:{enumerable:!0,configurable:!0,writable:!0,value(){let e=r.call(this);return e.shadowRoot&&O(e.shadowRoot),e}}});let i=new WeakMap;function a(e){Object.defineProperties(e.prototype,{popoverTargetElement:{enumerable:!0,configurable:!0,set(e){if(null===e)this.removeAttribute("popovertarget"),i.delete(this);else if(e instanceof Element)this.setAttribute("popovertarget",""),i.set(this,e);else throw TypeError("popoverTargetElement must be an element or null")},get(){if("button"!==this.localName&&"input"!==this.localName||"input"===this.localName&&"reset"!==this.type&&"image"!==this.type&&"button"!==this.type||this.disabled||this.form&&"submit"===this.type)return null;let e=i.get(this);if(e&&e.isConnected)return e;if(e&&!e.isConnected)return i.delete(this),null;let t=v(this),o=this.getAttribute("popovertarget");return(t instanceof Document||t instanceof k)&&o&&t.getElementById(o)||null}},popoverTargetAction:{enumerable:!0,configurable:!0,get(){let e=(this.getAttribute("popovertargetaction")||"").toLowerCase();return"show"===e||"hide"===e?e:"toggle"},set(e){this.setAttribute("popovertargetaction",e)}}})}a(HTMLButtonElement),a(HTMLInputElement);(e=document).addEventListener("click",e=>{let t=e.composedPath(),o=t[0];if(!(o instanceof Element)||o?.shadowRoot)return;let n=v(o);if(!(n instanceof k||n instanceof Document))return;let r=t.find(e=>e.matches?.("[popovertargetaction],[popovertarget]"));if(r){!function(e){let t=e.popoverTargetElement;if(!(t instanceof HTMLElement))return;let o=c(t);"show"===e.popoverTargetAction&&"showing"===o||("hide"!==e.popoverTargetAction||"hidden"!==o)&&("showing"===o?y(t,!0,!0):d(t,!1)&&(f.set(t,e),w(t)))}(r),e.preventDefault();return}}),e.addEventListener("keydown",e=>{let t=e.key,o=e.target;!e.defaultPrevented&&o&&("Escape"===t||"Esc"===t)&&T(o.ownerDocument,!0,!0)}),e.addEventListener("pointerdown",M),e.addEventListener("pointerup",M),O(document)}o.d(t,{apply:()=>C,injectStyles:()=>O,isPolyfilled:()=>P,isSupported:()=>H})}}]);
//# sourceMappingURL=3750-3ae2aba791d21f36-283b5304674a0185.js.map