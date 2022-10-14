"use strict";(self.webpackChunkwebsite=self.webpackChunkwebsite||[]).push([[1477],{10:t=>{t.exports=JSON.parse('{"blogPosts":[{"id":"wayland","metadata":{"permalink":"/blog/wayland","source":"@site/blog/2022-10-14-wayland.md","title":"Wayland is just Painful","description":"Wayland is the most painful thing I worked with when developing Qt application,","date":"2022-10-14T00:00:00.000Z","formattedDate":"October 14, 2022","tags":[{"label":"wayland","permalink":"/blog/tags/wayland"},{"label":"gnome","permalink":"/blog/tags/gnome"}],"readingTime":1.345,"hasTruncateMarker":false,"authors":[{"name":"Divya Antony J.R","title":"Maintainer of Spirit","url":"https://antonyjr.in","imageURL":"https://github.com/antony-jr.png","key":"ant"}],"frontMatter":{"slug":"wayland","title":"Wayland is just Painful","authors":"ant","tags":["wayland","gnome"]},"nextItem":{"title":"Welcome","permalink":"/blog/welcome"}},"content":"Wayland is the most painful thing I worked with when developing Qt application,\\nseriously Wayland is surely not a replacement for Xorg, it simply breaks working\\napplications.\\n\\nI know Xorg is old, but still I have no issue using it even though it\'s slow.\\n\\nLet\'s come to the topic, I was struggling to support Spirit for Wayland, at least for\\nGNOME Shell (Wayland) but it is just so painful. I\'m not complaining about getting the\\nactive window under Wayland, that is just fine with a lot of workarounds.\\n\\nBut imagine building your Qt application without using ```move()``` and ```resize()``` methods\\nin QWidget. For those who don\'t know about Qt development, these methods are extremely important, \\nat least for Spirit.\\n\\n```move()``` method in the QWidget class allows a QWidget to move to a specific co-ordinate in\\nthe screen, ```resize()``` helps to resize a QWidget to some dimension. Guess what Wayland does\\nnot allow a application to modify it\'s own position and dimenstion. I mean why?? GNOME developers\\nclaims it\'s for security reason. \\n\\nWithout ```move()``` method I can\'t find a way to move the Spirit window around, without resize\\nwe can\'t scale. Hell I can\'t even center my dialog boxes under Wayland.\\n\\nSo the only solution exists is that, use **gnome-shell-extension** and use QProcess to call gdbus\\nand get active window (which is fine), move our own Spirit widget (not fine, it\'s just painful)\\nand resize Spirit widget.\\n\\nSo we have to call system commands to stuff I did with no issue on X11 with Qt framework. So support\\nfor Wayland is going to take a long time folks. I\'m sorry."},{"id":"welcome","metadata":{"permalink":"/blog/welcome","source":"@site/blog/2022-10-09-welcome.md","title":"Welcome","description":"I\'m the author of Spirit, and Docusaurus v2 seems to provide a free blog. So I thought why not use it.","date":"2022-10-09T00:00:00.000Z","formattedDate":"October 9, 2022","tags":[{"label":"hello","permalink":"/blog/tags/hello"},{"label":"hola","permalink":"/blog/tags/hola"}],"readingTime":0.725,"hasTruncateMarker":false,"authors":[{"name":"Divya Antony J.R","title":"Maintainer of Spirit","url":"https://antonyjr.in","imageURL":"https://github.com/antony-jr.png","key":"ant"}],"frontMatter":{"slug":"welcome","title":"Welcome","authors":"ant","tags":["hello","hola"]},"prevItem":{"title":"Wayland is just Painful","permalink":"/blog/wayland"}},"content":"I\'m the author of Spirit, and Docusaurus v2 seems to provide a free blog. So I thought why not use it.\\nThis project was on my head for very long time but I just kept avoiding it, finally I can now release \\na decent version of my idea. This idea is not inspired from MaCoPix or Shimeji but found the both when \\npeople mentioned it at Reddit,\\n\\nAnyways Spirit is going to be very different from both, but obviously very similar to MaCoPix than\\nShimeji but kind of like a mix. But I don\'t think Spirit will be playing all over your desktop which\\ncan be messy. I like tha approach of using only the active window rather than the entire desktop.\\n\\nSo this is the first blog post to welcome everyone, I will be posting some tricks and tips on how\\nto use Spirit."}]}')}}]);