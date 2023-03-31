"use strict";(self.webpackChunkwebsite=self.webpackChunkwebsite||[]).push([[3301],{3905:(e,t,n)=>{n.d(t,{Zo:()=>u,kt:()=>y});var o=n(7294);function a(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function r(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);t&&(o=o.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,o)}return n}function i(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?r(Object(n),!0).forEach((function(t){a(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):r(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function l(e,t){if(null==e)return{};var n,o,a=function(e,t){if(null==e)return{};var n,o,a={},r=Object.keys(e);for(o=0;o<r.length;o++)n=r[o],t.indexOf(n)>=0||(a[n]=e[n]);return a}(e,t);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);for(o=0;o<r.length;o++)n=r[o],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(a[n]=e[n])}return a}var s=o.createContext({}),p=function(e){var t=o.useContext(s),n=t;return e&&(n="function"==typeof e?e(t):i(i({},t),e)),n},u=function(e){var t=p(e.components);return o.createElement(s.Provider,{value:t},e.children)},c="mdxType",d={inlineCode:"code",wrapper:function(e){var t=e.children;return o.createElement(o.Fragment,{},t)}},m=o.forwardRef((function(e,t){var n=e.components,a=e.mdxType,r=e.originalType,s=e.parentName,u=l(e,["components","mdxType","originalType","parentName"]),c=p(n),m=a,y=c["".concat(s,".").concat(m)]||c[m]||d[m]||r;return n?o.createElement(y,i(i({ref:t},u),{},{components:n})):o.createElement(y,i({ref:t},u))}));function y(e,t){var n=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var r=n.length,i=new Array(r);i[0]=m;var l={};for(var s in t)hasOwnProperty.call(t,s)&&(l[s]=t[s]);l.originalType=e,l[c]="string"==typeof e?e:a,i[1]=l;for(var p=2;p<r;p++)i[p]=n[p];return o.createElement.apply(null,i)}return o.createElement.apply(null,n)}m.displayName="MDXCreateElement"},9999:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>s,contentTitle:()=>i,default:()=>d,frontMatter:()=>r,metadata:()=>l,toc:()=>p});var o=n(7462),a=(n(7294),n(3905));const r={slug:"wayland",title:"Wayland is just Painful",authors:"ant",tags:["wayland","gnome"]},i=void 0,l={permalink:"/spirit/blog/wayland",source:"@site/blog/2022-10-14-wayland.md",title:"Wayland is just Painful",description:"Wayland is the most painful thing I worked with when developing Qt application,",date:"2022-10-14T00:00:00.000Z",formattedDate:"October 14, 2022",tags:[{label:"wayland",permalink:"/spirit/blog/tags/wayland"},{label:"gnome",permalink:"/spirit/blog/tags/gnome"}],readingTime:1.345,hasTruncateMarker:!1,authors:[{name:"Divya Antony J.R",title:"Maintainer of Spirit",url:"https://antonyjr.in",imageURL:"https://github.com/antony-jr.png",key:"ant"}],frontMatter:{slug:"wayland",title:"Wayland is just Painful",authors:"ant",tags:["wayland","gnome"]},nextItem:{title:"Welcome",permalink:"/spirit/blog/welcome"}},s={authorsImageUrls:[void 0]},p=[],u={toc:p},c="wrapper";function d(e){let{components:t,...n}=e;return(0,a.kt)(c,(0,o.Z)({},u,n,{components:t,mdxType:"MDXLayout"}),(0,a.kt)("p",null,"Wayland is the most painful thing I worked with when developing Qt application,\nseriously Wayland is surely not a replacement for Xorg, it simply breaks working\napplications."),(0,a.kt)("p",null,"I know Xorg is old, but still I have no issue using it even though it's slow."),(0,a.kt)("p",null,"Let's come to the topic, I was struggling to support Spirit for Wayland, at least for\nGNOME Shell (Wayland) but it is just so painful. I'm not complaining about getting the\nactive window under Wayland, that is just fine with a lot of workarounds."),(0,a.kt)("p",null,"But imagine building your Qt application without using ",(0,a.kt)("inlineCode",{parentName:"p"},"move()")," and ",(0,a.kt)("inlineCode",{parentName:"p"},"resize()")," methods\nin QWidget. For those who don't know about Qt development, these methods are extremely important,\nat least for Spirit."),(0,a.kt)("p",null,(0,a.kt)("inlineCode",{parentName:"p"},"move()")," method in the QWidget class allows a QWidget to move to a specific co-ordinate in\nthe screen, ",(0,a.kt)("inlineCode",{parentName:"p"},"resize()")," helps to resize a QWidget to some dimension. Guess what Wayland does\nnot allow a application to modify it's own position and dimenstion. I mean why?? GNOME developers\nclaims it's for security reason. "),(0,a.kt)("p",null,"Without ",(0,a.kt)("inlineCode",{parentName:"p"},"move()")," method I can't find a way to move the Spirit window around, without resize\nwe can't scale. Hell I can't even center my dialog boxes under Wayland."),(0,a.kt)("p",null,"So the only solution exists is that, use ",(0,a.kt)("strong",{parentName:"p"},"gnome-shell-extension")," and use QProcess to call gdbus\nand get active window (which is fine), move our own Spirit widget (not fine, it's just painful)\nand resize Spirit widget."),(0,a.kt)("p",null,"So we have to call system commands to stuff I did with no issue on X11 with Qt framework. So support\nfor Wayland is going to take a long time folks. I'm sorry."))}d.isMDXComponent=!0}}]);