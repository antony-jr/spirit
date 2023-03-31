"use strict";(self.webpackChunkwebsite=self.webpackChunkwebsite||[]).push([[4161],{3905:(e,t,i)=>{i.d(t,{Zo:()=>p,kt:()=>h});var n=i(7294);function o(e,t,i){return t in e?Object.defineProperty(e,t,{value:i,enumerable:!0,configurable:!0,writable:!0}):e[t]=i,e}function r(e,t){var i=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),i.push.apply(i,n)}return i}function a(e){for(var t=1;t<arguments.length;t++){var i=null!=arguments[t]?arguments[t]:{};t%2?r(Object(i),!0).forEach((function(t){o(e,t,i[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(i)):r(Object(i)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(i,t))}))}return e}function s(e,t){if(null==e)return{};var i,n,o=function(e,t){if(null==e)return{};var i,n,o={},r=Object.keys(e);for(n=0;n<r.length;n++)i=r[n],t.indexOf(i)>=0||(o[i]=e[i]);return o}(e,t);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);for(n=0;n<r.length;n++)i=r[n],t.indexOf(i)>=0||Object.prototype.propertyIsEnumerable.call(e,i)&&(o[i]=e[i])}return o}var l=n.createContext({}),c=function(e){var t=n.useContext(l),i=t;return e&&(i="function"==typeof e?e(t):a(a({},t),e)),i},p=function(e){var t=c(e.components);return n.createElement(l.Provider,{value:t},e.children)},d="mdxType",u={inlineCode:"code",wrapper:function(e){var t=e.children;return n.createElement(n.Fragment,{},t)}},f=n.forwardRef((function(e,t){var i=e.components,o=e.mdxType,r=e.originalType,l=e.parentName,p=s(e,["components","mdxType","originalType","parentName"]),d=c(i),f=o,h=d["".concat(l,".").concat(f)]||d[f]||u[f]||r;return i?n.createElement(h,a(a({ref:t},p),{},{components:i})):n.createElement(h,a({ref:t},p))}));function h(e,t){var i=arguments,o=t&&t.mdxType;if("string"==typeof e||o){var r=i.length,a=new Array(r);a[0]=f;var s={};for(var l in t)hasOwnProperty.call(t,l)&&(s[l]=t[l]);s.originalType=e,s[d]="string"==typeof e?e:o,a[1]=s;for(var c=2;c<r;c++)a[c]=i[c];return n.createElement.apply(null,a)}return n.createElement.apply(null,i)}f.displayName="MDXCreateElement"},7154:(e,t,i)=>{i.r(t),i.d(t,{assets:()=>l,contentTitle:()=>a,default:()=>u,frontMatter:()=>r,metadata:()=>s,toc:()=>c});var n=i(7462),o=(i(7294),i(3905));const r={sidebar_position:2},a="Edition 2021 Specification",s={unversionedId:"spirit-files/edition2021",id:"spirit-files/edition2021",title:"Edition 2021 Specification",description:'This document specifies the 2021 edition specification of the "Spirit File". The following',source:"@site/docs/spirit-files/edition2021.md",sourceDirName:"spirit-files",slug:"/spirit-files/edition2021",permalink:"/spirit/docs/spirit-files/edition2021",draft:!1,tags:[],version:"current",sidebarPosition:2,frontMatter:{sidebar_position:2},sidebar:"tutorialSidebar",previous:{title:"Introduction",permalink:"/spirit/docs/spirit-files/intro"},next:{title:"Download Spirit Files",permalink:"/spirit/docs/spirit-files/download"}},l={},c=[{value:"Actions Directory (actions/)",id:"actions-directory-actions",level:2},{value:"Audio Directory (audio/)",id:"audio-directory-audio",level:2},{value:"Meta JSON (meta.json)",id:"meta-json-metajson",level:2},{value:"name",id:"name",level:3},{value:"edition",id:"edition",level:3},{value:"version",id:"version",level:3},{value:"author",id:"author",level:3},{value:"copyright",id:"copyright",level:3},{value:"actions",id:"actions",level:3},{value:"Examples",id:"examples",level:2}],p={toc:c},d="wrapper";function u(e){let{components:t,...i}=e;return(0,o.kt)(d,(0,n.Z)({},p,i,{components:t,mdxType:"MDXLayout"}),(0,o.kt)("h1",{id:"edition-2021-specification"},"Edition 2021 Specification"),(0,o.kt)("p",null,'This document specifies the 2021 edition specification of the "Spirit File". The following\nis the directory structure to be followed when creating an 2021 edition Spirit File\ndirectory.'),(0,o.kt)("pre",null,(0,o.kt)("code",{parentName:"pre",className:"language-bash"},"-- <Spirit File Directory Name>\n |\n |-- actions\n | |\n | |-- default.webp # (REQUIRED) \n | |-- _default.webp # (OPTIONAL)\n | \n |-- audio # (OPTIONAL)\n | |\n | |-- track01.mp3 # (OPTIONAL)\n | -\n |\n |-- meta.json # (REQUIRED)\n -\n")),(0,o.kt)("p",null,'The "Spirit File Directory Name" can be any valid directory name, but it is recommended\nto use some meaningful name, for example if this is a mascot for Inori from Guilty Crown.\nThen it might be good to name the directory ',(0,o.kt)("inlineCode",{parentName:"p"},"Inori"),"."),(0,o.kt)("h2",{id:"actions-directory-actions"},"Actions Directory (actions/)"),(0,o.kt)("p",null,"This directory holds all the animated webp files for each action of the spirit, a default\naction is ",(0,o.kt)("strong",{parentName:"p"},"required")," to make the spirit valid. You can use traditional 2D animation\nsoftware to make different actions for your mascot. Webp files can also have specific\ndelays between frames and lot more customization. But this webp file should have a\ntransparent background."),(0,o.kt)("p",null,(0,o.kt)("strong",{parentName:"p"},"The actions/ directory should contain atleast the default.webp file which will be\nused to render the spirit when it is loaded by the user.")),(0,o.kt)("p",null,"A file with underscore before it's name corresponds to the bottom version of the\nanimation of some action. For example _default.webp is used when spirit is placed at\nthe bottom of the window. This allows the spirit file maker to use different animation\nfile when the mascot is at the bottom, like creating a floor to sit on or chair or some\nextra animation."),(0,o.kt)("p",null,"Only webp files are supported."),(0,o.kt)("h2",{id:"audio-directory-audio"},"Audio Directory (audio/)"),(0,o.kt)("p",null,"This directory is ",(0,o.kt)("strong",{parentName:"p"},"optional")," and can be avoided, this directory is used to store all\nyour mp3 audio files in case you need to play some tune or music for specific actions\nof your spirit."),(0,o.kt)("p",null,"Might be useful to create amazing animations with music. Like a music video animated\nwith the mascot playing a music."),(0,o.kt)("h2",{id:"meta-json-metajson"},"Meta JSON (meta.json)"),(0,o.kt)("p",null,"This is the main file which describes everything about the spirit like copyright\ninformation and how actions are defined, and positions and offsets. Example of a\nmeta.json file is shown below."),(0,o.kt)("pre",null,(0,o.kt)("code",{parentName:"pre",className:"language-js"},'{\n   "name": "Default", // Name of the Spirit\n   "edition": "2021", // Spirit File Directory Specification\n   "version": "v0.1.0", // Version of this Spirit File\n   "author": "Antony J.R", // Author of this Spirit File\n   "copyright": "Copyright (C) Giphy", // Copyright of Artwork used.\n   "actions": {\n           "default": { // Corresponds to actions in actions/ directory\n              "yoff": 40, // Y offset to offset the mascot when sitting on top of window\n              "scale": 40 // Scale of the mascot in percentage (here 40%)\n           }\n   }\n}\n')),(0,o.kt)("h3",{id:"name"},"name"),(0,o.kt)("p",null,"Specifies the fancy name of this Spirit, something like ",(0,o.kt)("strong",{parentName:"p"},'"Naruto Uzumaki"')," in case\nthe spirit displays a mascot of Naruto. This value can be anything will be displayed\nin the Spirit application when the user opens it. ",(0,o.kt)("strong",{parentName:"p"},"This field is required.")),(0,o.kt)("h3",{id:"edition"},"edition"),(0,o.kt)("p",null,'Specifies the edition which you followed to create the "Spirit File Directory",\n',(0,o.kt)("a",{parentName:"p",href:"https://github.com/antony-jr/ritual"},"ritual")," will use this information to check and\nvalidate your directory. Currently the possible value is ",(0,o.kt)("inlineCode",{parentName:"p"},"2021")," only. ",(0,o.kt)("strong",{parentName:"p"},"This field\nis required.")),(0,o.kt)("h3",{id:"version"},"version"),(0,o.kt)("p",null,"Specifies the version of this spirit file, this can be any string, but we recommend to\nuse semver. ",(0,o.kt)("strong",{parentName:"p"},"This field is required.")),(0,o.kt)("h3",{id:"author"},"author"),(0,o.kt)("p",null,"Specifies the author of this spirit file, not any copyright owners of the artwork, only\nthe author of this spirit file. ",(0,o.kt)("strong",{parentName:"p"},"This filed is required"),"."),(0,o.kt)("h3",{id:"copyright"},"copyright"),(0,o.kt)("p",null,"Copyright string to display to the users which should include attribution to the author\nof the artwork used by this Spirit file or any other license information. If in public\ndomain you may relicense. ",(0,o.kt)("strong",{parentName:"p"},"This filed is required"),"."),(0,o.kt)("h3",{id:"actions"},"actions"),(0,o.kt)("p",null,"This object specifies a list of json objects which corresponds to each file in the\nactions directory. The json object will define specific options for the action like\ny-axis offset, x-axis offset, scale, speed or next action to go after this action\nor loop. ",(0,o.kt)("strong",{parentName:"p"},"The default action is by default looped.")),(0,o.kt)("p",null,"Each action should have the ",(0,o.kt)("strong",{parentName:"p"},"file name of the webp file placed in actions/ directory\nas it's key to correspond.")," Since default.wep is required, the actions json object\nshould have a default json object to specify the options for the default animation."),(0,o.kt)("p",null,"Each object under action can have the following options,"),(0,o.kt)("pre",null,(0,o.kt)("code",{parentName:"pre",className:"language-js"},'"actions" : {\n  "default" : { // REQUIRED\n      "xoff": 0, // INTEGER    | used when spirit is on top of window\n      "yoff": 0, // INTEGER    | used when spirit is on top of window\n      "_xoff": 0, // INTEGER   | used when spirit is on bottom of window\n      "_yoff": 0, // INTEGER   | used when spirit is on bottom of window\n      "loop": true, // BOOLEAN | loop the action (true by default for default action)\n      "scale": 100, // INTEGER | defines the percentage of scale (here, 100%)\n      "speed": 100, // INTEGER | defines the percentage of speed (here, 100%)\n      "next": "", // STRING | when loop is false, show this action next\n      "play": "track0", // STRING | mp3 file to play when showing this action (OPTIONAL)\n      "delay": 1000, // INTEGER | delay in miliseconds when moving to next action  \n  }\n}\n')),(0,o.kt)("p",null,(0,o.kt)("strong",{parentName:"p"},"All options shown above are totally optional but you should give a empty object\nwith the key corresponding to the filename of the webp file in actions directory.")),(0,o.kt)("h2",{id:"examples"},"Examples"),(0,o.kt)("p",null,"You can look at examples at ",(0,o.kt)("a",{parentName:"p",href:"https://github.com/antony-jr/ritual"},"ritual")," under sample/\ndirectory in the root of the repo."))}u.isMDXComponent=!0}}]);