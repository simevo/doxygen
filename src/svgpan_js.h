"/**\n"
" * The code below is based on SVGPan Library 1.2 and was modified for doxygen\n"
" * to support both zooming and panning via the mouse and via embedded bottons.\n"
" *\n"
" * This code is licensed under the following BSD license:\n"
" *\n"
" * Copyright 2009-2010 Andrea Leofreddi <a.leofreddi@itcharm.com>. All rights reserved.\n"
" * \n"
" * Redistribution and use in source and binary forms, with or without modification, are\n"
" * permitted provided that the following conditions are met:\n"
" * \n"
" *    1. Redistributions of source code must retain the above copyright notice, this list of\n"
" *       conditions and the following disclaimer.\n"
" * \n"
" *    2. Redistributions in binary form must reproduce the above copyright notice, this list\n"
" *       of conditions and the following disclaimer in the documentation and/or other materials\n"
" *       provided with the distribution.\n"
" * \n"
" * THIS SOFTWARE IS PROVIDED BY Andrea Leofreddi ``AS IS'' AND ANY EXPRESS OR IMPLIED\n"
" * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND\n"
" * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Andrea Leofreddi OR\n"
" * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n"
" * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"
" * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON\n"
" * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n"
" * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF\n"
" * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
" * \n"
" * The views and conclusions contained in the software and documentation are those of the\n"
" * authors and should not be interpreted as representing official policies, either expressed\n"
" * or implied, of Andrea Leofreddi.\n"
" */\n"
"\n"
"var root = document.documentElement;\n"
"var state = 'none';\n"
"var stateOrigin;\n"
"var stateTf = root.createSVGMatrix();\n"
"var cursorGrab = ' url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAMAAAAolt3jAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAAlQTFRFAAAA////////c3ilYwAAAAN0Uk5T//8A18oNQQAAAD1JREFUeNp0zlEKACAIA9Bt9z90bZBZkQj29qFBEuBOzQHSnWTTyckEfqUuZgFvslH4ch3qLCO/Kr8cAgwATw4Ax6XRCcoAAAAASUVORK5CYII=\"), move';\n"
"var zoomSteps = 10;\n"
"var zoomInFactor;\n"
"var zoomOutFactor;\n"
"var windowWidth;\n"
"var windowHeight;\n"
"var svgDoc;\n"
"var minZoom;\n"
"var maxZoom;\n"
"if (!window) window=this;\n"
"\n"
"/**\n"
" * Show the graph in the middle of the view, scaled to fit \n"
" */\n"
"function show()\n"
"{\n"
"  if (window.innerHeight) // Firefox\n"
"  {\n"
"    windowWidth = window.innerWidth;\n"
"    windowHeight = window.innerHeight;\n"
"  }\n"
"  else if (document.documentElement.clientWidth) // Chrome/Safari\n"
"  {\n"
"    windowWidth = document.documentElement.clientWidth\n"
"    windowHeight = document.documentElement.clientHeight\n"
"  }\n"
"  if (!windowWidth || !windowHeight) // failsafe\n"
"  {\n"
"    windowWidth = 800;\n"
"    windowHeight = 600;\n"
"  }\n"
"  minZoom = Math.min(Math.min(viewHeight,windowHeight)/viewHeight,Math.min(viewWidth,windowWidth)/viewWidth);\n"
"  maxZoom = minZoom+1.5;\n"
"  zoomInFactor = Math.pow(maxZoom/minZoom,1.0/zoomSteps);\n"
"  zoomOutFactor = 1.0/zoomInFactor;\n"
"\n"
"  var g = svgDoc.getElementById('viewport');\n"
"  try\n"
"  {\n"
"    var bb = g.getBBox(); // this can throw an exception if css { display: none }\n"
"    var tx = (windowWidth-viewWidth*minZoom+8)/(2*minZoom);\n"
"    var ty = viewHeight+(windowHeight-viewHeight*minZoom)/(2*minZoom);\n"
"    var a = 'scale('+minZoom+') rotate(0) translate('+tx+' '+ty+')';\n"
"    g.setAttribute('transform',a);\n"
"  }\n"
"  catch(e) {}\n"
"}\n"
"\n"
"/**\n"
" * Register handlers\n"
" */\n"
"function init(evt) \n"
"{\n"
"  svgDoc = evt.target.ownerDocument;\n"
"  if (top.window && top.window.registerShow) // register show function in html doc for dynamic sections\n"
"  {\n"
"    top.window.registerShow(sectionId,show);\n"
"  }\n"
"  show();\n"
"\n"
"  setAttributes(root, {\n"
"     \"onmousedown\" : \"handleMouseDown(evt)\",\n"
"     \"onmousemove\" : \"handleMouseMove(evt)\",\n"
"     \"onmouseup\"   : \"handleMouseUp(evt)\"\n"
"  });\n"
"\n"
"  if (window.addEventListener)\n"
"  {\n"
"    if (navigator.userAgent.toLowerCase().indexOf('webkit') >= 0 || \n"
"        navigator.userAgent.toLowerCase().indexOf(\"opera\") >= 0 || \n"
"        navigator.appVersion.indexOf(\"MSIE\") != -1)\n"
"    {\n"
"      window.addEventListener('mousewheel', handleMouseWheel, false); // Chrome/Safari/IE9\n"
"    }\n"
"    else\n"
"    {\n"
"      window.addEventListener('DOMMouseScroll', handleMouseWheel, false); // Others\n"
"    }\n"
"  }\n"
"}\n"
"\n"
"window.onresize=function()\n"
"{\n"
"  if (svgDoc) { show(); }\n"
"}\n"
"\n"
"/**\n"
" * Instance an SVGPoint object with given event coordinates.\n"
" */\n"
"function getEventPoint(evt) \n"
"{\n"
"  var p = root.createSVGPoint();\n"
"  p.x = evt.clientX;\n"
"  p.y = evt.clientY;\n"
"  return p;\n"
"}\n"
"\n"
"/**\n"
" * Sets the current transform matrix of an element.\n"
" */\n"
"function setCTM(element, matrix) \n"
"{\n"
"  var s = \"matrix(\" + matrix.a + \",\" + matrix.b + \",\" + matrix.c + \",\" + matrix.d + \",\" + matrix.e + \",\" + matrix.f + \")\";\n"
"  element.setAttribute(\"transform\", s);\n"
"}\n"
"\n"
"/**\n"
" * Sets attributes of an element.\n"
" */\n"
"function setAttributes(element, attributes)\n"
"{\n"
"  for (i in attributes)\n"
"    element.setAttributeNS(null, i, attributes[i]);\n"
"}\n"
"\n"
"function doZoom(g,point,zoomFactor)\n"
"{\n"
"  var p = point.matrixTransform(g.getCTM().inverse());\n"
"  var k = root.createSVGMatrix().translate(p.x, p.y).scale(zoomFactor).translate(-p.x, -p.y);\n"
"  var n = g.getCTM().multiply(k);\n"
"  var s = Math.max(n.a,n.d);\n"
"  if      (s>maxZoom) n=n.translate(p.x,p.y).scale(maxZoom/s).translate(-p.x,-p.y);\n"
"  else if (s<minZoom) n=n.translate(p.x,p.y).scale(minZoom/s).translate(-p.x,-p.y);\n"
"  setCTM(g, n);\n"
"  stateTf = stateTf.multiply(n.inverse());\n"
"}\n"
"\n"
"/**\n"
" * Handle mouse move event.\n"
" */\n"
"function handleMouseWheel(evt) \n"
"{\n"
"  if (!evt) evt = window.evt;\n"
"  if (!evt.shiftKey) return; // only zoom when shift is pressed\n"
"  if (evt.preventDefault) evt.preventDefault();\n"
"  evt.returnValue = false;\n"
"\n"
"  if (state!='pan')\n"
"  {\n"
"    var delta;\n"
"    if (evt.wheelDelta)\n"
"    {\n"
"      delta = evt.wheelDelta / 7200; // Opera/Chrome/IE9/Safari\n"
"    }\n"
"    else\n"
"    {\n"
"      delta = evt.detail / -180; // Mozilla\n"
"    }\n"
"    var svgDoc = evt.target.ownerDocument;\n"
"    var g = svgDoc.getElementById(\"viewport\");\n"
"    var p = getEventPoint(evt);\n"
"    doZoom(g,p,1+delta);\n"
"  }\n"
"}\n"
"\n"
"/**\n"
" * Handle mouse move event.\n"
" */\n"
"function handleMouseMove(evt) \n"
"{\n"
"  if(evt.preventDefault)\n"
"    evt.preventDefault();\n"
"\n"
"  evt.returnValue = false;\n"
"\n"
"  var g = svgDoc.getElementById(\"viewport\");\n"
"\n"
"  if (state == 'pan') \n"
"  {\n"
"    // Pan mode\n"
"    var p = getEventPoint(evt).matrixTransform(stateTf);\n"
"    setCTM(g,stateTf.inverse().translate(p.x - stateOrigin.x, p.y - stateOrigin.y));\n"
"  } \n"
"}\n"
"\n"
"/**\n"
" * Handle click event.\n"
" */\n"
"function handleMouseDown(evt) \n"
"{\n"
"  if(evt.preventDefault)\n"
"    evt.preventDefault();\n"
"  evt.returnValue = false;\n"
"  var g = svgDoc.getElementById(\"viewport\");\n"
"  state = 'pan';\n"
"  stateTf = g.getCTM().inverse();\n"
"  stateOrigin = getEventPoint(evt).matrixTransform(stateTf);\n"
"  g.style.cursor = cursorGrab;\n"
"}\n"
"\n"
"/**\n"
" * Handle mouse button release event.\n"
" */\n"
"function handleMouseUp(evt) \n"
"{\n"
"  if (evt.preventDefault) evt.preventDefault();\n"
"  evt.returnValue = false;\n"
"  var g = svgDoc.getElementById(\"viewport\");\n"
"  g.style.cursor = \"default\";\n"
"  // Quit pan mode\n"
"  state = '';\n"
"}\n"
"\n"
"/**\n"
" * Dumps a matrix to a string (useful for debug).\n"
" */\n"
"function dumpMatrix(matrix) \n"
"{\n"
"  var s = \"[ \" + matrix.a + \", \" + matrix.c + \", \" + matrix.e + \"\\n  \" + matrix.b + \", \" + matrix.d + \", \" + matrix.f + \"\\n  0, 0, 1 ]\";\n"
"  return s;\n"
"}\n"
"\n"
"/**\n"
" * Handler for pan buttons\n"
" */\n"
"function handlePan(x,y)\n"
"{\n"
"  var g = svgDoc.getElementById(\"viewport\");\n"
"  setCTM(g,g.getCTM().translate(x*20/minZoom,y*20/minZoom));\n"
"}\n"
"\n"
"/**\n"
" * Handle reset button\n"
" */\n"
"function handleReset()\n"
"{\n"
"  show();\n"
"}\n"
"\n"
"/**\n"
" * Handler for zoom buttons\n"
" */\n"
"function handleZoom(evt,direction)\n"
"{\n"
"  var g = svgDoc.getElementById(\"viewport\");\n"
"  var factor = direction=='in' ? zoomInFactor : zoomOutFactor;\n"
"  var m = g.getCTM();\n"
"  var p = root.createSVGPoint();\n"
"  p.x = windowWidth/2;\n"
"  p.y = windowHeight/2;\n"
"  doZoom(g,p,factor);\n"
"}\n"
"\n"
"function serializeXmlNode(xmlNode) \n"
"{\n"
"  if (typeof window.XMLSerializer != \"undefined\") {\n"
"    return (new window.XMLSerializer()).serializeToString(xmlNode);\n"
"  } else if (typeof xmlNode.xml != \"undefined\") {\n"
"    return xmlNode.xml;\n"
"  }\n"
"  return \"\";\n"
"}\n"
"\n"
"/** \n"
" * Handler for print function\n"
" */\n"
"function handlePrint(evt)\n"
"{\n"
"  evt.returnValue = false;\n"
"  var g = svgDoc.getElementById(\"graph\");\n"
"  var xs = serializeXmlNode(g);\n"
"  try {\n"
"    var w = window.open('about:blank','_blank','width='+windowWidth+',height='+windowHeight+\n"
"                        ',toolbar=0,status=0,menubar=0,scrollbars=0,resizable=0,location=0,directories=0');\n"
"    var d = w.document;\n"
"    d.write('<html xmlns=\"http://www.w3.org/1999/xhtml\" '+\n"
"            'xmlns:svg=\"http://www.w3.org/2000/svg\" '+\n"
"            'xmlns:xlink=\"http://www.w3.org/1999/xlink\">');\n"
"    d.write('<head><title>Print SVG</title></head>');\n"
"    d.write('<body style=\"margin: 0px; padding: 0px;\" onload=\"window.print(); window.close();\">');\n"
"    d.write('<div id=\"svg\" style=\"width:'+windowWidth+'px; height:'+windowHeight+'px;\">'+xs+'</div>');\n"
"    d.write('</body>');\n"
"    d.write('</html>');\n"
"    d.close();\n"
"  } catch(e) {\n"
"    alert('Print function not supported by this browser!');\n"
"  }\n"
"}\n"
"\n"
"\n"
"\n"
"\n"