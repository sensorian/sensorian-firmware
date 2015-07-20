#! /usr/bin/env python

def classify_token(tkn):
	if tkn == '"':
		return "QUOTE"
	elif tkn == ' ':
		return "SPACE"
	else:
		return "NORMAL"

def scratch_message_parse(msg):
	parsed_message = []
	new_token = ""
	parser_state = "NORMAL"
	for char in msg:
		if parser_state == "NORMAL":
			tkn_class = classify_token(char)
			if tkn_class == "NORMAL":
				new_token += char
			elif tkn_class == "SPACE":
				if new_token != "":
					parsed_message.append(new_token)
				new_token = ""
			elif tkn_class == "QUOTE":
				parser_state = "QUOTED"

		elif parser_state == "QUOTED":
			tkn_class = classify_token(char)
			if tkn_class == "QUOTE":
				if new_token != "":
					parsed_message.append(new_token)
				new_token = ""
				parser_state = "NORMAL"
			else:
				new_token += char
	return parsed_message

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"
