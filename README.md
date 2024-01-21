<h2 align="center">AysyncElegantOTA-Custom-HTML</h2>
<p align="center">
 This fork was <b>made from an already archived and deprecated version</b>!<br>
 See the implications of the original author below.
</p> 
<p>
 Credits to the original author of this library, which allows very easy implementation into projects.<br>
 Unfortunately it does not allow to make adjustments to the User Interface. 
 (The original sources contain only a gzipped HTML page.)<br>
 Hint: The offered ElegantOTA Pro Version promises more features, but I did not test it.
</p>
<p align="center"><b>Goal of this fork</b></p>
<p>
 Extend the AsyncElegantOTA.<b>begin()</b> signature.<br>
 We now have parameters for the HTML page, it's size and a flag wether the page data is gzipped.<br>
 (Works still with the original page as a default, see examples).
</p>
<p>
 Provide a <b>plain HTML/Javascript page in simpleWebpage.h</b> as an alternate UI which may easily adopted to specific needs.<br>
 (Notice: This UI does currently not generate a MD5 checksum. As a consequence the update process skips MD5 checksum verification.)
</p>
<p>New example <b>AsyncGui_Demo</b> making use of the new feature.</p>
<span>
<img src="/docs/simpleGui1.png" width="300">
<img src="/docs/simpleGui2.png" width="300">
<img src="/docs/simpleGui3.png" width="300">
</span>
<hr>
<p align="center"><img src="/docs/logo.svg?sanitize=true&raw=true" width="400"></p>

 
<h2 align="center">Deprecated</h2>

<p align="center">
AsyncElegantOTA has been deprecated in favor of new ElegantOTA V3 which now comes with Async Mode and latest features. Please consider moving to ElegantOTA V3 for further updates and improvements. <a href="https://github.com/ayushsharma82/ElegantOTA">Learn More</a>
</p>

<p align="center">
Existing users/supporters, please star the latest ElegantOTA repository so that more people can know about it :) Thanks!
</p>
