
#define UTILH
#ifdef UTILH

void save_screenshot(string filename, int w, int h)
{

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int nSize = w*h * 3;
	char* dataBuffer = (char*)malloc(nSize*sizeof(char));

	if (!dataBuffer) return;


	glReadPixels((GLint)0, (GLint)0,
		(GLint)w, (GLint)h,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, dataBuffer);

	int timest = (unsigned)time(NULL);
	string ts;
	ostringstream convert;
	convert << timest;
	ts = convert.str();
	filename = filename + "_" + ts + ".tga";
	FILE *filePtr = fopen(filename.c_str(), "wb");
	if (!filePtr) return;


	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char header[6] = { w % 256, w / 256,
		h % 256, h / 256,
		24, 0 };
	// We write the headers
	fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
	fwrite(header, sizeof(unsigned char), 6, filePtr);
	// And finally our image data
	fwrite(dataBuffer, sizeof(GLubyte), nSize, filePtr);
	fclose(filePtr);

	free(dataBuffer);


}





#endif