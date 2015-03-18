Mimics the WritePrivateProfileString and GetPrivateProfileString Win32 API functions to read and write a string to an INI file.  Includes unit tests, and was written using test-driven development.

WritePrivateProfileString:
DESCRIPTION:  Writes a string to an INI file.

PARAMETERS:
pAppName (IN) Points to a null-terminated string containing the name of the section to which the string will be copied. If the section does not exist, it is created. The name of the section is case-independent; the string can be any combination of uppercase and lowercase letters.

pKeyName (IN) Points to the null-terminated string containing the name of the key to be associated with a string. If the key does not exist in the specified section, it is created. If this parameter is NULL, the entire section, including all entries within the section, is deleted.

pString (IN) Points to a null-terminated string to be written to the file. If this parameter is NULL, the key pointed to by the pKeyName parameter is deleted.

pFileName (IN) Points to a null-terminated string that names the initialization file.

RETURN:       If the function successfully copies the string to the initialization file, the return value is nonzero. If the function fails, or if it flushes the cached version of the most recently accessed initialization file, the return value is zero. To get extended error information, call GetLastError.

NOTES:        If all three parameters are NULL, the function flushes the cache. The function always returns FALSE after flushing the cache, regardless of whether the flush succeeds or fails.

GetPrivateProfileString

DESCRIPTION:  Processes the INI file.

PARAMETERS:
pAppName (IN) Points to a null-terminated string that specifies the section containing the key name.If this parameter is NULL, the GetPrivateProfileString function copies all section names in the file to the supplied buffer.

pKeyName (IN) Pointer to the null-terminated string containing the key name whose associated string is to be retrieved. If this parameter is NULL, all key names in the section specified by the lpAppName parameter are copied to the buffer specified by the pReturnedString parameter.

pDefault (IN) Pointer to a null-terminated default string. If the pKeyName key cannot be found in the initialization file, GetPrivateProfileString copies the default string to the pReturnedString buffer. This parameter cannot be NULL. Avoid specifying a default string with trailing blank characters. The function inserts a null character in the pReturnedString buffer to strip any trailing blanks.

pReturnedString (OUT) Pointer to the buffer that receives the retrieved string.

nSize (IN) Specifies the size, in characters, of the buffer pointed to by the pReturnedString parameter.

pFileName (IN) Pointer to a null-terminated string that names the initialization file. If this parameter does not contain a full path to the file, Windows searches for the file in the Windows directory.

RETURN:       If the function succeeds, the return value is the number of characters copied to the buffer, not including the terminating null character. If neither pAppName nor pKeyName is NULL and the supplied destination buffer is too small to hold the requested string, the string is truncated and followed by a null character, and the return value is equal to nSize minus one. If either pAppName or pKeyName is NULL and the supplied destination buffer is too small to hold all the strings, the last string is truncated and followed by two null characters. In this case, the return value is equal to nSize minus two.

NOTES:        The GetPrivateProfileString function searches the specified initialization file for a key that matches the name specified by the pKeyName parameter under the section heading specified by the pAppName parameter. If it finds the key, the function copies the corresponding string to the buffer. If the key does not exist, the function copies the default character string specified by the pDefault parameter. A section in the initialization file must have the following form:
[section](section.md)
key=string
> .
> .
> .

If pAppName is NULL, GetPrivateProfileString copies all section names in the specified file to the supplied buffer.
If pKeyName is NULL, the function copies all key names in the specified section to the supplied buffer. An application can use this method to enumerate all of the sections and keys in a file. In either case, each string is followed by a null character and the final string is followed by a second null character. If the supplied destination buffer is too small to hold all the strings, the last string is truncated and followed by two null characters. If the string associated with pKeyName is enclosed in single or double quotation marks, the marks are discarded when the GetPrivateProfileString function retrieves the string. The GetPrivateProfileString function is not case-sensitive; the strings can be a combination of uppercase and lowercase letters.