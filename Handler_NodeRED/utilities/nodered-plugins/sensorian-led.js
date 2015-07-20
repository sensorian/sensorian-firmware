/****************************************************************************
 * Copyright (C) 2015 Sensorian                                             *
 * @author Michael Lescisin                                                 *
 *                                                                          *
 *                                                                          *
 * This file is part of Sensorian.                                          *
 *                                                                          *
 *   Sensorian is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Sensorian is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Sensorian.                                          *
 *   If not, see <http://www.gnu.org/licenses/>.                            *
 ****************************************************************************/

module.exports = function(RED) {
	var exec = require('child_process').exec;
	function SensorianLedNode(config) {
		RED.nodes.createNode(this,config);
		var node = this;
		this.on('input', function(msg) {
			//Just debugging for now
			//console.log(msg);
			if (msg.payload == 1)
			{
				console.log("Sensorian LED should be ON");
				exec("LED_ON");
			}
			else if (msg.payload == 0)
			{
				console.log("Sensorian LED should be OFF");
				exec("LED_OFF");
			}
		});
	}
	RED.nodes.registerType("sensorian-led",SensorianLedNode);
}
