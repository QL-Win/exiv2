// ***************************************************************** -*- C++ -*-
// exifprint.cpp
// Sample program to print the Exif metadata of an image

#include <exiv2/exiv2.hpp>

#include <iostream>

typedef Exiv2::ExifData::const_iterator (*EasyAccessFct)(const Exiv2::ExifData& ed);

#define EXPORT extern "C" __declspec(dllexport)

std::string escapeXML(const std::string& str)
{
	std::ostringstream result;

	for (auto i : str)
	{
		if (i == '<') result << "&lg;";
		else if (i == '>') result << "&gt;";
		else result << i;
	}

	return result.str();
}

void appendXML(std::ostringstream& s, const std::string& key, const std::string& label,
               const std::vector<std::string>& value)
{
	const auto lbl = escapeXML(label);

	s << "  <" << key << " Label=\"" << lbl << "\">";

	for (const auto& v : value)
		s << escapeXML(v);

	s << "</" << key << ">" << std::endl;
}

std::string printTag(const Exiv2::ExifData& exifData, const std::string& key)
{
	std::ostringstream tmp;

	const Exiv2::ExifKey ek(key);
	const auto md = exifData.findKey(ek);
	if (md != exifData.end())
		md->write(tmp, &exifData);

	return tmp.str();
}

std::string printTag(const Exiv2::ExifData& exifData, EasyAccessFct easyAccessFct)
{
	std::ostringstream tmp;

	const auto md = easyAccessFct(exifData);
	if (md != exifData.end())
		md->write(tmp, &exifData);

	return tmp.str();
}

std::string formatXML(const Exiv2::Image::AutoPtr& image, const Exiv2::ExifData& exifData)
{
	std::ostringstream result;

	result << "<Exif>" << std::endl;

	appendXML(result, "_.MIME", "MIME Type", {image->mimeType()});
	appendXML(result, "_.Size", "Image Size",
	          {std::to_string(image->pixelWidth()), " x ", std::to_string(image->pixelHeight())});

	if (!exifData.empty())
	{
		appendXML(result, "Exif.Image.Make", "Camera make", {printTag(exifData, "Exif.Image.Make")});
		appendXML(result, "Exif.Image.Model", "Camera model", {printTag(exifData, "Exif.Image.Model")});
		appendXML(result, "Exif.Photo.LensModel", "Lens model", {printTag(exifData, Exiv2::lensName)});
		appendXML(result, "Exif.Image.DateTimeOriginal", "Image timestamp",
		          {printTag(exifData, "Exif.Image.DateTimeOriginal")});
		appendXML(result, "Exif.Photo.ExposureTime", "Exposure time",
		          {
			          exifData.findKey(Exiv2::ExifKey("Exif.Photo.ExposureTime")) != exifData.end()
				          ? printTag(exifData, "Exif.Photo.ExposureTime")
				          : printTag(exifData, "Exif.Photo.ShutterSpeedValue")
		          });
		appendXML(result, "Exif.Photo.ApertureValue", "Aperture",
		          {
			          exifData.findKey(Exiv2::ExifKey("Exif.Photo.FNumber")) != exifData.end()
				          ? printTag(exifData, "Exif.Photo.FNumber")
				          : printTag(exifData, "Exif.Photo.ApertureValue")
		          });
		appendXML(result, "Exif.Photo.ExposureBiasValue", "Exposure bias",
		          {printTag(exifData, "Exif.Photo.ExposureBiasValue")});
		appendXML(result, "Exif.Photo.Flash", "Flash", {printTag(exifData, "Exif.Photo.Flash")});
		appendXML(result, "Exif._.FlashBias", "Flash bias", {printTag(exifData, Exiv2::flashBias)});
		appendXML(result, "Exif.Photo.FocalLength", "Focal length", {printTag(exifData, Exiv2::focalLength)});
		if (exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLengthIn35mmFilm")) != exifData.end())
			appendXML(result, "Exif.Photo.FocalLengthIn35mmFilm", "Focal length (35 mm eq.)",
			          {printTag(exifData, "Exif.Photo.FocalLengthIn35mmFilm")});
		appendXML(result, "Exif.Photo.SubjectDistance", "Subject distance",
		          {printTag(exifData, Exiv2::subjectDistance)});
		appendXML(result, "Exif.Photo.ISOSpeedRatings", "ISO speed", {printTag(exifData, Exiv2::isoSpeed)});
		appendXML(result, "Exif.Photo.ExposureProgram", "Exposure mode", {printTag(exifData, Exiv2::exposureMode)});
		appendXML(result, "Exif.Photo.MeteringMode", "Metering mode", {printTag(exifData, Exiv2::meteringMode)});
		appendXML(result, "Exif._.Macro", "Macro mode", {printTag(exifData, Exiv2::macroMode)});
		appendXML(result, "Exif._.Quality", "Image quality", {printTag(exifData, Exiv2::imageQuality)});
		appendXML(result, "Exif._.WhiteBalance", "White balance", {printTag(exifData, Exiv2::whiteBalance)});
		appendXML(result, "Exif.Image.Orientation", "Orientation", {printTag(exifData, "Exif.Image.Orientation")});
	}

	result << "</Exif>" << std::endl;
	return result.str();
}

EXPORT int32_t GetExif(wchar_t* file, char* buffer)
try
{
	auto image = Exiv2::ImageFactory::open(file);
	if (image.get() == nullptr)
		return -1;

	image->readMetadata();
	auto& exifData = image->exifData();

	const auto xml = formatXML(image, exifData);

	if (buffer != nullptr)
		strcpy_s(buffer, xml.size() + 1, xml.c_str());

	return xml.size();
}
catch (...)
{
	return -1;
}

EXPORT int32_t GetOrientation(wchar_t* file)
try
{
	auto image = Exiv2::ImageFactory::open(file);
	if (image.get() == nullptr)
		return -1;

	image->readMetadata();
	auto& exifData = image->exifData();

	if (exifData.empty())
		return 1;

	const auto key = exifData.findKey(Exiv2::ExifKey("Exif.Image.Orientation"));
	if (key == exifData.end())
		return 1;

	return std::stoi(key->value().toString());
}
catch (...)
{
	return 1;
}

EXPORT int32_t GetThumbnail(wchar_t* file, uint8_t* buffer)
try
{
	auto image = Exiv2::ImageFactory::open(file);
	if (image.get() == nullptr)
		return -1;

	image->readMetadata();
	const Exiv2::PreviewManager pvMgr(*image);

	auto pvList = pvMgr.getPreviewProperties();
	if (pvList.empty())
		return -1;

	auto target = 0;
	for (uint32_t i = 0; i < pvList.size(); i++)
		if (pvList[i].height_ >= pvList[target].height_)
			target = i;

	const auto len = pvList[target].size_;

	if (buffer == nullptr)
		return len;

	memcpy_s(buffer, len, pvMgr.getPreviewImage(pvList[target]).pData(), len);

	return len;
}
catch (...)
{
	return -1;
}
