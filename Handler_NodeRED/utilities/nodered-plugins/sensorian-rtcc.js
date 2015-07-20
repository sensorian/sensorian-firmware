/**
 * Copyright 2013,2014 IBM Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

/**
 * Modifications Copyright 2015 Sensorian
 * @author Michael Lescisin
 **/

module.exports = function(RED) {
	"use strict";
	var reconnectTime = RED.settings.socketReconnectTime||10000;
	var socketTimeout = RED.settings.socketTimeout||null;
	var net = require('net');

	var connectionPool = {};
	
	
	function SensorianRTCCAlarmTriggerNode(n) {
		RED.nodes.createNode(this,n);
		//this.host = n.host;
		this.host = "127.0.0.1";
		//this.port = n.port * 1;
		this.port = 7777;
		this.topic = n.topic;
		//this.stream = (!n.datamode||n.datamode=='stream'); /* stream,single*/
		this.stream = true;
		//this.datatype = n.datatype||'buffer'; /* buffer,utf8,base64 */
		this.datatype = "utf8";
		this.newline = (n.newline||"").replace("\\n","\n").replace("\\r","\r");
		this.base64 = n.base64;
		this.server = (typeof n.server == 'boolean')?n.server:(n.server == "server");
		this.closing = false;
		this.connected = false;
		var node = this;
		var count = 0;

		var buffer = null;
		var client;
		var reconnectTimeout;
		var end = false;
		var setupTcpClient = function() {
			node.log("connecting to "+node.host+":"+node.port);
			node.status({fill:"grey",shape:"dot",text:"connecting"});
			var id = (1+Math.random()*4294967295).toString(16);
			client = net.connect(node.port, node.host, function() {
				buffer = (node.datatype == 'buffer')? new Buffer(0):"";
				node.connected = true;
				node.log("connected to "+node.host+":"+node.port);
				node.status({fill:"green",shape:"dot",text:"connected"});
				});
				
				connectionPool[id] = client;

				client.on('data', function (data) {
					if (node.datatype != 'buffer')
					{
						data = data.toString(node.datatype);
					}
					if (node.stream)
					{
						console.log("Sending message from here");
						console.log(data);
						var dictionary_info = JSON.parse(data.split('\n')[0]);
						console.log("BEGIN dictionary_info");
						console.log(dictionary_info);
						console.log("END dictionary_info");
						if ('alarmtrigger' in dictionary_info)
						{
							console.log("BEGIN dictionary_info['alarmtrigger']");
							console.log(dictionary_info['alarmtrigger']);
							console.log("END dictionary_info['alarmtrigger']");
							//var msg = {topic:node.topic, payload:data};
							var msg = {topic:node.topic, payload:dictionary_info.alarmtrigger.toString()};
							msg._session = {type:"tcp",id:id};
							node.send(msg);
						}
					}
					else
					{
						if ((typeof data) === "string")
						{
							buffer = buffer+data;
						}
						else
						{
							buffer = Buffer.concat([buffer,data],buffer.length+data.length);
						}
					}
				});
				client.on('end', function() {
					if (!node.stream || (node.datatype == "utf8" && node.newline != "" && buffer.length > 0))
					{
						var msg = {topic:node.topic,payload:buffer};
						msg._session = {type:"tcp",id:id};
						if (buffer.length !== 0)
						{
							end = true; // only ask for fast re-connect if we actually got something
							node.send(msg);
						}
						buffer = null;
					}
				});
				client.on('close', function()
				{
					delete connectionPool[id];
					node.connected = false;
					node.status({fill:"red",shape:"ring",text:"disconnected"});
					if (!node.closing)
					{
						if (end)
						{ // if we were asked to close then try to reconnect once very quick.
							end = false;
							reconnectTimeout = setTimeout(setupTcpClient, 20);
						}
						else
						{
							node.log("connection lost to "+node.host+":"+node.port);
							reconnectTimeout = setTimeout(setupTcpClient, reconnectTime);
						}
					}
					else
					{
						if (node.done) { node.done(); }
					}
				});
				
				client.on('error', function(err) {
					node.log(err);
				});
			}
			setupTcpClient();

			this.on('close', function(done) {
				node.done = done;
				this.closing = true;
				client.end();
				clearTimeout(reconnectTimeout);
				if (!node.connected) { done(); }
			});
	}
	RED.nodes.registerType("sensorian-rtcc.alarmtrigger",SensorianRTCCAlarmTriggerNode);
}
