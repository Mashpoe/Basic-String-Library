//
//  str.c
//  CHeckScript
//
//  Created by Mashpoe on 2/28/19.
//  Copyright © 2019 Mashpoe. All rights reserved.
//


#include "str.h"
#include <stdio.h>
#include <string.h>

struct string {
	size_t alloc; // stores the number of bytes allocated
	size_t length;
	char* data;
};

string* STRING_CREATE(const char* str) {
	string* s = malloc(sizeof(string));
	
	if (str != NULL) {
		size_t strLength = strlen(str);
		s->alloc = strLength + 1;
		s->data = (char*)malloc(s->alloc);
		memcpy(s->data, str, strLength);
		s->data[strLength] = '\0';
		s->length = strLength;
	} else {
		// initially allocate 1 byte for an empty string's null terminator
		s->alloc = 1;
		s->data = (char*)malloc(s->alloc);
		s->data[0] = '\0';
		// length is still zero, the null terminator doesn't count
		s->length = 0;
	}
	
	return s;
}

void STRING_FREE(string* s) {
	free(s->data);
	free(s);
}

char* STRING_AT(string* s) {
	return s->data;
}

const char* STRING_GET(string* s) {
	return s->data;
}

size_t STRING_SIZE(string* s) {
	return s->length;
}

char* STRING_GET_DATA(string* s) {
	return s->data;
}

size_t STRING_GET_ALLOC(string* s) {
	return s->alloc;
}

char* STRING_REALLOCATE(string* s) {
	// because of the null terminator, s->alloc cannot be zero and therefore can be doubled
	s->alloc *= 2;
	char* newdata = (char*)malloc(s->alloc);
	memcpy(newdata, s->data, s->length);
	if (s->data != NULL) {
		free(s->data);
	}
	s->data = newdata;
	return newdata;
}

bool STRING_HAS_SPACE(string* s) {
	// allocate based on s->length + 1 to account for the null terminator
	return s->alloc - (s->length + 1) > 0;
}

void STRING_ADD(string* s, char c) {
	char* data = STRING_HAS_SPACE(s) ?
	s->data:
	STRING_REALLOCATE(s);
	data[s->length++] = c;
	data[s->length] = '\0'; // add the new null terminator
}

void STRING_REMOVE(string* s, size_t pos, size_t len) {
	// anyone who puts in a bad index can face the consequences on their own
	memmove(&s->data[pos], &s->data[pos+len], s->length - pos);
	s->length -= len;
	s->data[s->length] = '\0';
}

void STRING_INSERT(string* s, size_t pos, const char* str) {
	
	size_t strLength = strlen(str);
	
	// allocate necessary space
	size_t newLength = s->length + strLength;
	
	// make sure there is enough room for new characters and null terminator
	if (s->alloc <= newLength + 1) {
		char* newdata = (char*)malloc(newLength+1);
		s->alloc = newLength + 1;
		
		memcpy(newdata, s->data, pos); // copy leading characters
		memcpy(&newdata[pos+strLength], &s->data[pos], s->length-pos); // copy trailing characters
		
		if (s->data != NULL) {
			free(s->data);
		}
		
		s->data = newdata;
		
	} else {
		memmove(&s->data[pos], &s->data[pos+strLength], s->length - pos); // move trailing characters
	}
	
	s->length = newLength;
	
	// copy str chars
	memcpy(&s->data[pos], str, strLength);
	
	s->data[newLength] = '\0'; // add new null terminator
}

void STRING_REPLACE(string*s, size_t pos, size_t len, const char* str) {
	
	size_t strLength = strlen(str);
	
	// allocate necessary space
	size_t newLength = s->length + strLength - len;
	
	// make sure there is enough room for new characters and null terminator
	if (s->alloc <= newLength + 1) {
		char* newdata = (char*)malloc(newLength+1);
		s->alloc = newLength + 1;
		
		memcpy(newdata, s->data, pos); // copy leading characters
		memcpy(&newdata[pos+strLength], &s->data[pos+len], s->length-pos-len); // copy trailing characters
		
		if (s->data != NULL) {
			free(s->data);
		}
		
		s->data = newdata;
		
	} else {
		memmove(&s->data[pos+strLength], &s->data[pos+len], s->length-pos-len); // move trailing characters
	}
	
	s->length = newLength;
	
	// copy str chars
	memcpy(&s->data[pos], str, strLength);
	
	s->data[newLength] = '\0'; // add new null terminator
}
