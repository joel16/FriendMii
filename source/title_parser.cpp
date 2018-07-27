#include <stdarg.h>
#include <tinyxml2.h>

extern "C"
{
	#include "title_parser.h"
}

using namespace tinyxml2;

static void log_func(const char *s, ...)
{
		char buf[256];
		va_list argptr;
		va_start(argptr, s);
		vsnprintf(buf, sizeof(buf), s, argptr);
		va_end(argptr);

		FILE *fp;
		fp = fopen("/log.txt", "a");
		fprintf(fp, buf);
		fclose(fp);
}

#define DEBUG(...) log_func(__VA_ARGS__)

XMLDocument doc;

void Title_ParserParseXML(const char *path)
{	
	if (doc.LoadFile(path))
		return;
	else
		DEBUG("Load pass: %s\n", path);
}

const char *Title_ParserGetValue(const char *element)
{
	XMLElement *root = doc.FirstChildElement("releases");
	XMLElement *child = root->FirstChildElement("releases");
	XMLElement *value = child->FirstChildElement(element);

	DEBUG("Element: %s\n", value->GetText());
	return value->GetText();
}