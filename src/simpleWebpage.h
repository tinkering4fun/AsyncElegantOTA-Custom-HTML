// simpleWebpage.h
// ---------------------------------------------------------------------
// This is a plain HTML/Javascript Webpage as a frontend 
// for AsyncElegantOTA w/o using a complex framework.
//
// As this is not a cryptic gzipped response page, it may be easily 
// copied and modified for more specific requirements.
//
// Notice
// This version does not yet support MD5 checksum generation!
//
// Be aware that AsyncElegantOTA is already deprecated!
// 
// tinkering4fun, January 2024
// ---------------------------------------------------------------------
#ifndef SimpleOTAWebpage_h
#define SimpleOTAWebpage_h


const char SIMPLE_HTML[]  PROGMEM = R"text(
<!DOCTYPE HTML><html>
<head>
  <title>ESP OTA Flash Tool</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  
   <!-- Stylesheet --------------------------------------- -->
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.5rem;
    color: white;
  }
  .header {
    overflow: hidden;
    background-color: #049eb3;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .section {
		border: solid;
		border-width: 2px;
		border-radius: 6px;
		border-color: #049eb3;
    padding: 10px;
    max-width: 600px;
    margin: 5px;
  }
  .group {
		border: none;
    padding: 10px;
    /* max-width: 600px; */
    margin: 15px;
  }
	input::file-selector-button {
		font-weight: bold;
		color: red;
		padding: 0.5em;
		border: thin solid grey;
		border-radius: 3px;
	}
	input[type=text] {
		width: 95%;
		font-size: 1rem;
		border: none;
		background-color: #f0f0f0;
	}
	.info{
		padding: 0.5em;
	}
	.button{
		border-radius: 10px;
	  background-color: #049eb3;
		font-size: 1rem;
		padding: 5px 10px;
		color: white;
		text-decoration: none;
	}
	button:disabled,
	button[disabled]{
	  border: 1px solid #999999;
	  background-color: #cccccc;
	  color: #666666;
	}
	
  </style>
  <!-- --------------------------------------------------- -->
  
</head>
<body>
	<div class="header">
    <h1>ESP OTA Flash Tool</h1>
	</div>
	<div class="content">
	
		<div class="section" style="horizontal-align:middle" >		
			<p>
				<b>Device</b><span class="info" id="chip">-</span><b>ID</b><span class="info" id ="chipid">-</span>
			</p>		
			<div>
				<b>Flash Area:</b>
				<input type="radio" id="firmware" name="fileType" value="firmware" checked=true>
				<label for="firmware">Firmware</label>
				<input type="radio" id="filesystem" name="fileType" value="filesystem">
				<label for="filesystem">Filesystem</label>
			</div>
		</div>	
		<div class="section" >	
			
			<form action="/update" method="post" enctype="multipart/form-data">

				<!-- Backend supports MD5 verification and expects this field.
						 As long as we do not support MD5 at the frontend, we set 
						 it to 'void' and a patch in the backend code handles this 
						 by simply skipping the MD5 check. 
				-->
				<input id="MD5" name="MD5" type="text"  style="display:none;"  value="void" /> 
	
				<!-- CUSTOMIZED FILE SELECTOR, trick found at
						 https://stackoverflow.com/questions/1944267/how-to-change-the-button-text-of-input-type-file
				-->			
			  <!-- Hide the standard file selector button -->
			  <input id="file" name="file" type="file" accept=".bin" style="display:none;" onchange="fileChange();" />
			  <!-- Connect a visible custom button to the hidden selector -->
			  <div>
					<p><b>Image File</b>
					<input class="button" type="button" id="loadfile" value="Select" onclick="document.getElementById('file').click();" />
					</p>
				</div>
								
				<!-- Selected file info -->
				<div class ="group">
					<table style="width:100%"><tr>
						<td><label for="filename">File:</label></td>
						<td><input type="text" id="filename" name="filename" readonly ></td>
					</tr><tr>
						<td><label for="filesize">Size:</label></td>
						<td><input type="text" id="filesize" name="filesize" readonly ></td>
					</tr><tr>						
						<td><label for="filetime">Time:</label></td>
						<td><input type="text" id="filetime" name="filetime" readonly ></td>			
					</tr></table>
				</div>			
			
				<!-- FLASH (enabled after file selected) -->
			  <button id="flash" disabled class="button" style="width:200px;">FLASH</button>
				<div>				
					<progress id="progress" value="0" max="100"  style="margin-top:10px"></progress> <span id="prozent"></span>
				</div>			
		</form>	

		</div>
		<!-- Footer -->
		<div class="section" style="border:none;">
			<hr>
			<input class="button"  type="button" onclick="location.href='./';" value="Home" />
	  </div>		
	</div>

  <!-- --------------------------------------------------- -->	
	<script>
		
	// Fetch info about target device
	// -------------------------------------------------------------------
	fetch("./update/identity")
  .then((response) => response.json())
  .then((json) => { 
		console.log(json);
		document.getElementById("chip").innerHTML = json.hardware;
		document.getElementById("chipid").innerHTML = Number(json.id).toString(16).toUpperCase() + " hex";
	});
		
	// Credits to Austin Gil for the handleSubmit() skeleton
	// See https://www.freecodecamp.org/news/upload-files-with-javascript/
	// -------------------------------------------------------------------
	const form = document.querySelector('form');
	form.addEventListener('submit', handleSubmit);
	
	// -------------------------------------------------------------------
	function handleSubmit(event) {

	  const form = event.currentTarget;
	  const url = new URL(form.action);
	  const formData = new FormData(form);
	  const searchParams = new URLSearchParams(formData);
	
	  const fetchOptions = {
	    method: form.method,
	  };
	
	  if (form.method.toLowerCase() === 'post') {
	    if (form.enctype === 'multipart/form-data') {
	      fetchOptions.body = formData;
	    } else {
	      fetchOptions.body = searchParams;
	    }
	  } else {
	    url.search = searchParams;
	  }
		
		// Filename reported to OTA server declares the file's type!
		if(document.getElementById("firmware").checked)
			formData.set("file", formData.get("file"), "firmware");
		else if(document.getElementById("filesystem").checked)
			formData.set("file", formData.get("file"), "filesystem");
		else {
			console.log("FILETYPE ERROR");
			return;
		}
		
		// Notice:
		// fetch() has currently (early 2024) no support for a progress bar
		// So we use the old XHR method instead
	  // fetch(url, fetchOptions);
	  xhrUpload(formData);
	
	  event.preventDefault();
	}
	

	// XHR upload concept taken from
	// https://jun711.github.io/web/how-to-listen-for-upload-progress-when-using-AJAX-XMLHttpRequest-XHR/
	// -------------------------------------------------------------------
	function xhrUpload(formdata) {
	
		console.log('XHR Upload start');
		
	  let xhr = new XMLHttpRequest();
	
	  // listen for upload progress
	  xhr.upload.onprogress = function(event) {
	    let percent = Math.round(100 * event.loaded / event.total);
	    //console.log(`File is ${percent} uploaded.`);
	    
	    document.getElementById("progress").value = percent;
			document.getElementById("prozent").innerHTML = percent + "%";	
	  };
	
	  // handle error
	  xhr.upload.onerror = function() {
	    console.log(`Error during the upload: ${xhr.status}.`);
	  };

	  // handle error
	  xhr.upload.onloadend = function() {
	    console.log(`Load end: ${xhr.status}.`);
	  };
	  	
	  // upload completed successfully
	  xhr.upload.onload = function() {
	    console.log('Upload completed successfully.');
	    
	    document.getElementById("flash").disabled = true;
	    document.getElementById("flash").innerHTML = "OK";
	    
	  };
	
		xhr.onreadystatechange = function() {
		
		console.log('XHR state ' + xhr.readyState);
     if(xhr.readyState==4) {
         console.log('XHR COMPLETE ' + xhr.responseText);
			}
     };

	  xhr.open('POST', '/update');
	  
	  xhr.send(formdata);
	}	
	
	// -------------------------------------------------------------------
	function fileChange()
	{
		//FileList Objekt aus dem Input Element mit der ID "fileA"
		var fileList = document.getElementById("file").files;
		
		//File Objekt (erstes Element der FileList)
		var file = fileList[0];
		
		//File Objekt nicht vorhanden = keine Datei ausgewählt oder vom Browser nicht unterstützt
		if(!file)
				return;
		
		// Show details
		document.getElementById("filename").value = file.name;
		document.getElementById("filesize").value = file.size + " bytes";
		
		var date = new Date(file.lastModified);
		document.getElementById("filetime").value = date.toLocaleTimeString() + " - " + date.toLocaleDateString(); //.type;
		
		// Check for binary file selected
		if( file.type != "application/x-extension-bin")
			return;
		
		// Enable FLASH button
		document.getElementById("progress").value = 0;
		document.getElementById("prozent").innerHTML = "0%";	
		document.getElementById("flash").innerHTML = "FLASH";
		document.getElementById("flash").disabled = false;
	}	
	
	</script>
	<!-- --------------------------------------------------- -->
</body>
</html>

)text";


const uint32_t SIMPLE_HTML_SIZE = sizeof(SIMPLE_HTML);

#endif
