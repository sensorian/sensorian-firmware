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
    
    function SensorianTftNode(n) {
        RED.nodes.createNode(this,n);
        //this.host = n.host;
        this.host = "127.0.0.1";
        //this.port = n.port * 1;
        this.port = 7778;
        this.base64 = n.base64;
        //this.doend = n.end || false;
        this.doend = true;
        //this.beserver = n.beserver;
        this.beserver = "client";
        this.name = n.name;
        this.closing = false;
        this.connected = false;
        this.orientation = n.orientation;
        this.name = n.name;
        var node = this;

        if (!node.beserver||node.beserver=="client")
        {
            var reconnectTimeout;
            var client = null;
            var end = false;

            var setupTcpClient = function() {
                node.log("connecting to "+node.host+":"+node.port);
                node.status({fill:"grey",shape:"dot",text:"connecting"});
                client = net.connect(node.port, node.host, function() {
                    node.connected = true;
                    node.log("connected to "+node.host+":"+node.port);
                    node.status({fill:"green",shape:"dot",text:"connected"});
                });
                
                client.on('error', function (err) {
                    node.log('error : '+err);
                });
                
                client.on('end', function (err) {
                });
                
                client.on('close', function() {
                    node.status({fill:"red",shape:"ring",text:"disconnected"});
                    node.connected = false;
                    client.destroy();
                    if (!node.closing)
                    {
                        if (end)
                        {
                            end = false;
                            reconnectTimeout = setTimeout(setupTcpClient,20);
                        }
                        else
                        {
                            node.log("connection lost to "+node.host+":"+node.port);
                            reconnectTimeout = setTimeout(setupTcpClient,reconnectTime);
                        }
                    }
                    else
                    {
                        if (node.done) { node.done(); }
                    }
                });
            }
            setupTcpClient();

            node.on("input", function(msg) {
                console.log("Name: " + this.name);
                console.log("Orientation: " + this.orientation);
                //Add this to the message payload
                msg.payload = this.orientation + msg.payload;
                if (node.connected && msg.payload != null)
                {
                    if (Buffer.isBuffer(msg.payload))
                    {
                        client.write(msg.payload);
                    }
                    else if (typeof msg.payload === "string" && node.base64)
                    {
                        client.write(new Buffer(msg.payload,'base64'));
                    }
                    else
                    {
                        client.write(new Buffer(""+msg.payload));
                    }
                    if (node.doend === true)
                    {
                        end = true;
                        client.end();
                    }
                }
            });

            node.on("close", function(done) {
                node.done = done;
                this.closing = true;
                client.end();
                clearTimeout(reconnectTimeout);
                if (!node.connected) { done(); }
            });
        }
    }   
    RED.nodes.registerType("sensorian-tft",SensorianTftNode);
}
