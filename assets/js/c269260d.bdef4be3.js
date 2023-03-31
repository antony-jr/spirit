"use strict";(self.webpackChunkwebsite=self.webpackChunkwebsite||[]).push([[2494],{3905:(e,t,r)=>{r.d(t,{Zo:()=>u,kt:()=>m});var n=r(7294);function i(e,t,r){return t in e?Object.defineProperty(e,t,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[t]=r,e}function o(e,t){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),r.push.apply(r,n)}return r}function a(e){for(var t=1;t<arguments.length;t++){var r=null!=arguments[t]?arguments[t]:{};t%2?o(Object(r),!0).forEach((function(t){i(e,t,r[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):o(Object(r)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(r,t))}))}return e}function s(e,t){if(null==e)return{};var r,n,i=function(e,t){if(null==e)return{};var r,n,i={},o=Object.keys(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||(i[r]=e[r]);return i}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(i[r]=e[r])}return i}var p=n.createContext({}),c=function(e){var t=n.useContext(p),r=t;return e&&(r="function"==typeof e?e(t):a(a({},t),e)),r},u=function(e){var t=c(e.components);return n.createElement(p.Provider,{value:t},e.children)},l="mdxType",d={inlineCode:"code",wrapper:function(e){var t=e.children;return n.createElement(n.Fragment,{},t)}},f=n.forwardRef((function(e,t){var r=e.components,i=e.mdxType,o=e.originalType,p=e.parentName,u=s(e,["components","mdxType","originalType","parentName"]),l=c(r),f=i,m=l["".concat(p,".").concat(f)]||l[f]||d[f]||o;return r?n.createElement(m,a(a({ref:t},u),{},{components:r})):n.createElement(m,a({ref:t},u))}));function m(e,t){var r=arguments,i=t&&t.mdxType;if("string"==typeof e||i){var o=r.length,a=new Array(o);a[0]=f;var s={};for(var p in t)hasOwnProperty.call(t,p)&&(s[p]=t[p]);s.originalType=e,s[l]="string"==typeof e?e:i,a[1]=s;for(var c=2;c<o;c++)a[c]=r[c];return n.createElement.apply(null,a)}return n.createElement.apply(null,r)}f.displayName="MDXCreateElement"},7807:(e,t,r)=>{r.r(t),r.d(t,{assets:()=>p,contentTitle:()=>a,default:()=>d,frontMatter:()=>o,metadata:()=>s,toc:()=>c});var n=r(7462),i=(r(7294),r(3905));const o={sidebar_position:4},a="CLI",s={unversionedId:"usage/CLI",id:"usage/CLI",title:"CLI",description:"The CLI can be used to control Spirits via the command line, this is for the sake of power users",source:"@site/docs/usage/CLI.md",sourceDirName:"usage",slug:"/usage/CLI",permalink:"/spirit/docs/usage/CLI",draft:!1,tags:[],version:"current",sidebarPosition:4,frontMatter:{sidebar_position:4},sidebar:"tutorialSidebar",previous:{title:"Window Quirks",permalink:"/spirit/docs/usage/quirks"},next:{title:"Spirit File",permalink:"/spirit/docs/category/spirit-file"}},p={},c=[],u={toc:c},l="wrapper";function d(e){let{components:t,...r}=e;return(0,i.kt)(l,(0,n.Z)({},u,r,{components:t,mdxType:"MDXLayout"}),(0,i.kt)("h1",{id:"cli"},"CLI"),(0,i.kt)("p",null,"The CLI can be used to control Spirits via the command line, this is for the sake of power users\nwho don't want to leave their terminal just to change your Spirit."),(0,i.kt)("pre",null,(0,i.kt)("code",{parentName:"pre"},"Spirit \ud83d\udc7b v1 commit-28f162a (build 103), The Twenty First Century Window Sitter.\nCopyright (C) 2021, D. Antony J.R <antonyjr@pm.me>.\n\nUsage: ./spirit-28f162a-x86_64.AppImage <SUBCOMMAND> [OPTIONS]\n       ./spirit-28f162a-x86_64.AppImage load <LOCAL SPIRIT FILE> [OPTIONS]\n       ./spirit-28f162a-x86_64.AppImage set-action <ACTION NAME> [OPTIONS]\n       ./spirit-28f162a-x86_64.AppImage set-property <Property=Value> ... [OPTIONS]\n       ./spirit-28f162a-x86_64.AppImage add-quirk name=<ProgramName> yoffset=<int> ... [OPTIONS]\n\n\nSUBCOMMAND:\n\n init                   Initialize spirit daemon.\n load                   Load a spirit from local file.\n info                   Show info on current loaded spirit file.\n set-action             Set current action of the loaded spirit.\n list-actions           List all actions of the loaded spirit.\n show-action            Show current action.\n quirks                 Show Quirks list.\n add-quirk              Add a Quirk.\n remove-quirk           Remove a Quirk.\n property               Show current action properties.\n set-property           Set properties for current spirit.\n reset-property         Reset properties to default.\n gh-load                Load a spirit file from github repo.\n deinit                 Quit spirit daemon.\n get-port               Get the port that spirit daemon uses.\n\nOPTIONS [init]:\n\n -d,--debug     Enable debug messages.\n")))}d.isMDXComponent=!0}}]);