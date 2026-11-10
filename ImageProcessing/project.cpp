#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
//#include <Windows.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utils/filesystem.hpp>
//#include <opencv2/corquality/qualitypsnr.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "libxl.h"
//#include "ImageManipulation.cpp"
#include "ImageEncryption.cpp"

using namespace libxl;
using namespace cv;
using namespace std;


#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include "QualityAssessment.cpp"
#include "ExportExcel.cpp"
#include "ImageDecryption.cpp"





using random_bytes_engine = std::independent_bits_engine<std::default_random_engine, CHAR_BIT, unsigned char>;

string generateImageName(string folderName, string encryptionCriteria, double psnr_value, Scalar SSIMValue, double mse_value) {
	return folderName + "/" + encryptionCriteria + "mse_" + std::to_string(mse_value) + "_PSNR_" + std::to_string(psnr_value) +
		"_RGB_" + std::to_string(SSIMValue.val[2] * 100) + "%_" + std::to_string(SSIMValue.val[1] * 100) + "%_" + std::to_string(SSIMValue.val[0] * 100) + +"%_" + ".bmp";
}
string generateEncryptedImageName(bool withMSB, bool withChannelSwap, bool withFisher, bool withBytesXOR, bool cbcMode) {
	string eim = "";
	if (withMSB) {
		eim += "M";
	}
	if (withChannelSwap) {
		eim += "C";
	}
	if (withFisher) {
		eim += "P";
	}
	if (withBytesXOR) {
		eim += "B";
	}
	if (cbcMode) {
		eim += "-CBC";
	}
	return eim;
}
int main()
{
	//https://stattrek.com/statistics/random-number-generator#table
	string prng;
	prng = "00011001010001110111100010101111101110110110011000100100101010000010111000010110000000001111001010000000100000100110111111100000000101101110001001100100110110010110011011001101110001011101100100100110100001010010000010001110011010001100111000101000010010101110001001001010100000101100101000010000100111101010011001101101000010100101010111001000000111000011100101000010010100001100100010110101111111010000001101110110111111001100001010011100111001111101000000101111001110011001010101001100110101110101111101010100";
	string modifiedPRNG = "10010010001001001010000111110111000111100000110000110110000011010111011111010110101100100110100011001001110011001110010111101000011001111110000111111110010110010101101001110010011010000101010110100011111000000111011111101010111100010010101111100101001001111110101111100100001100110100111111011000111100111000110011011101011001110111101001101011100100000110101001101110110001010111000101011110110000010101001111110110100111110001110110001110011101101110000100011011110000010010111100110110010001001010111111101110";

	//string imageName[] = { "LenaRGB","barbara","Elaine512" };
	string imageName[] = { "LenaRGB" };
	//string imageName[] = { "lena1bit"};
	string imageType = ".bmp";
	//const int IV = 234;

	for (int imageIndex = 0; imageIndex < (sizeof(imageName) / sizeof(imageName[0])); imageIndex++)
	{
		// definations
		string imagePath = "C:/Users/issa_/source/repos/PPU/PPU/images/bmp/" + imageName[imageIndex] + imageType;
		//string imagePath = "C:/Users/issa_/source/repos/PPU/PPU/images/bmp/lenaencryptedoriginal" + imageType;		
		//string imagePathTwo = "C:/Users/issa_/source/repos/PPU/PPU/images/bmp/" + imageType;		
		ImageManipulation image(512, 64, 64, cv::imread(imagePath, IMREAD_UNCHANGED), imageName[imageIndex], imageName[imageIndex]);
		ImageManipulation originalImage = image;

		// Divide iamge into blocks

		// generate and save the image edges using laplacian
		image.getLaplacian(imageIndex);

		// create image directory for saving the output
		cv::utils::fs::createDirectory(image.FolderName);






		int visitedBlocks = 0;
		int visitedBlocksEdge = 0;

		std::string blockId = "1";
		//ImageManipulation image(4096, 64, 64, originalImage.Image.clone(), image.FolderName, image.ImageName + "_Block_" + blockId);
		//ImageManipulation image(512, 64, 64, originalImage.Image.clone(), image.FolderName, image.ImageName + "_Block_" + blockId);


		//encrypt the ege block
		bool withMSB = true;			//	M
		bool withChannelSwap = true;	//	C
		bool withFisher = true;			//	P
		bool withBytesXOR = true;		//	B
		bool cbcMode = true;			// -CBC
		string process = "";
		process = generateEncryptedImageName(withMSB, withChannelSwap, withFisher, withBytesXOR, cbcMode);

		std::vector<cv::Mat> blocks;
		image.divideImage(blocks);
		ImageManipulation xoredBlockMan;
		int** fisherRandomNumbersBlocks[64];
		cv::Mat iv_img;





		for (int i = 0; i < blocks.size(); i++) {
			ImageManipulation block(64, 8, 8, blocks[i], image.FolderName, image.ImageName + "_Block_" + blockId);
			ImageEncryption currentblockEncryption(block, prng);

			cv::Mat xoredBlock;
			cv::Mat previous_block;
			cv::Mat current_block;

			current_block = blocks[i];

			

			if (cbcMode)
			{
				if (i - 1 >= 0) {
					previous_block = blocks[i - 1];
				}
				else {
					iv_img = cv::Mat(blocks[i].rows, blocks[i].cols, CV_8UC3);
					randu(iv_img, Scalar(0, 0, 0), Scalar(255, 255, 255));
					previous_block = iv_img;
				}
				xoredBlock = currentblockEncryption.xorWithPreviousBlock(previous_block, current_block);
			}
			else {
				xoredBlock = current_block;
			}

			xoredBlockMan = ImageManipulation(64, 8, 8, xoredBlock, image.FolderName, image.ImageName + "_XOREDBlock_" + blockId);
			ImageEncryption xoredBlockEncryption(xoredBlockMan, prng);

			xoredBlockEncryption.encryptImage(withMSB, withChannelSwap);
			if (withFisher)
			{
				fisherRandomNumbersBlocks[i] = xoredBlockEncryption.encryptImageFisher();
			}

			if (withBytesXOR)
			{
				xoredBlockEncryption.encryptImage_alter_fisher();
			}
			blocks[i] = xoredBlockEncryption.Image.Image;
		}


		cv::Mat encryptedImage = image.gatherImage(blocks);
		ImageManipulation encryptedImageM(512, 64, 64, encryptedImage, imageName[imageIndex], imageName[imageIndex]);
		ImageEncryption ecnrypt(encryptedImageM, prng);




		QualityAssessment qa;

		double edgePsnr = qa.getPSNR(originalImage.Image, ecnrypt.Image.Image);
		double mse = qa.getMSE(originalImage.Image, ecnrypt.Image.Image);
		Scalar edgeScalar = qa.getMSSIM(originalImage.Image, ecnrypt.Image.Image);
		std::string originalImgName = originalImage.FolderName + "/original.bmp";
		std::string edgeEncryImgName = generateImageName(originalImage.FolderName, process + "_ONLY_", edgePsnr, edgeScalar, mse);
		imwrite(originalImgName, originalImage.Image);
		imwrite(edgeEncryImgName, ecnrypt.Image.Image);



		//qa.calculateEntropyHistogram(ecnrypt.Image.Image);
		//qa.histogramRGB(ecnrypt.Image.Image);
		qa.histogramRGB(originalImage.Image);
		double cr = 0;
		float fr = 0;
		//correlation is not accurate

		cr  = qa.correlation(originalImage.Image, originalImage.Image);
		fr  = qa.correlationFloat(originalImage.Image, originalImage.Image);

		cr = qa.correlation(ecnrypt.Image.Image, originalImage.Image);
		fr = qa.correlationFloat(ecnrypt.Image.Image, originalImage.Image);


		ImageDecryption decryption(ecnrypt.Image.Image.clone(), prng);
		//ImageDecryption decryption(ecnrypt.Image.Image.clone(), modifiedPRNG);

		Book* bookXOR = xlCreateBook();
		ExportExcel xorExcelExport(bookXOR);
		xorExcelExport.exportPostPreEncryptionToExcel(0, 0, "EncryptedDecrypted", originalImage.Image.clone(), decryption.Image.clone());
		bookXOR->save(xorExcelExport.convertStringToTCHAR(imageName[imageIndex]));
		bookXOR->save(L"EncryptDecrypt.xls");
		bookXOR->release();

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////// Decryption Section ////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////


		std::vector<cv::Mat> encryptedBlocks = blocks;
		std::vector<cv::Mat> decryptedBlocks;
		//image.divideImage(encryptedBlocks);






		decryptedBlocks = std::vector<cv::Mat>();
		for (int j = 0; j < encryptedBlocks.size(); j++) {
			ImageDecryption decryptionBlock(encryptedBlocks[j].clone(), prng);



			if (withBytesXOR)
			{
				decryptionBlock.decryptImage_alter_fisher(ecnrypt.AlterRandomNumbers, ecnrypt.IV_alter);
			}

			if (withFisher)
			{
				decryptionBlock.decryptFisherYates(fisherRandomNumbersBlocks[j]);
			}

			if (withMSB || withChannelSwap)
			{
				decryptionBlock.decryptImageMSB(ecnrypt.IV, ecnrypt.RandomNumbers, withChannelSwap, withMSB);
			}

			cv::Mat xoredBlock;
			cv::Mat previous_block;
			cv::Mat current_block;
			current_block = decryptionBlock.Image;

			if (j - 1 >= 0) {
				previous_block = blocks[j - 1];
			}
			else {
				previous_block = iv_img;
			}
			if (cbcMode)
			{
				xoredBlock = decryptionBlock.xorWithPreviousBlock(previous_block, current_block);
			}
			else {
				xoredBlock = current_block;
			}


			decryptedBlocks.push_back(xoredBlock.clone());


		}

		cv::Mat decryptedImage = image.gatherImage(decryptedBlocks);
		std::string decryptedImgName = originalImage.FolderName + "/decryptedMSB.bmp";
		imwrite(decryptedImgName, decryptedImage);

		/*cv::Mat decryptedImage = image.gatherImage(decryptedBlocks);
		std::string decryptedImgName = originalImage.FolderName + "/decryptedMSB.bmp";
		imwrite(decryptedImgName, decryptedImage);

		if (withBytesXOR)
		{

			cv::Mat beforeXOR;
			beforeXOR = decryption.Image.clone();
			decryption.decryptImage_alter_fisher(ecnrypt.AlterRandomNumbers, ecnrypt.IV_alter);
			std::string decryptedImgName = originalImage.FolderName + "/decryptedAlter.bmp";

			imwrite(decryptedImgName, decryption.Image);


			Book* bookXOR = xlCreateBook();
			ExportExcel xorExcelExport(bookXOR);
			xorExcelExport.exportPostPreEncryptionToExcel(0, 0, "withBytesXOR", beforeXOR, decryption.Image.clone());
			bookXOR->save(xorExcelExport.convertStringToTCHAR(imageName[imageIndex]));
			bookXOR->save(L"EncryptDecryptXOR.xls");
			bookXOR->release();

		}*/

		/*if (withFisher)
		{
			cv::Mat beforeFisher;
			beforeFisher = decryption.Image.clone();
			decryption.decryptFisherYates(ecnrypt.FisherRandomNumbers);
			std::string decryptedImgName = originalImage.FolderName + "/decryptedFisher.bmp";
			imwrite(decryptedImgName, decryption.Image);


			Book* bookFisher = xlCreateBook();
			ExportExcel fisherExcelExport(bookFisher);
			fisherExcelExport.exportPostPreEncryptionToExcel(0, 1, "withFisher", beforeFisher, decryption.Image.clone());
			bookFisher->save(fisherExcelExport.convertStringToTCHAR(imageName[imageIndex]));
			bookFisher->save(L"EncryptDecryptFisher.xls");
			bookFisher->release();

		}*/

		/*if (withMSB || withChannelSwap)
		{
			cv::Mat beforeMSB;
			beforeMSB = decryption.Image.clone();
			decryption.decryptImageMSB(ecnrypt.IV, ecnrypt.RandomNumbers, withChannelSwap, withMSB);
			std::string decryptedImgName = originalImage.FolderName + "/decryptedMSB.bmp";

			imwrite(decryptedImgName, decryption.Image);

			Book* bookMSB = xlCreateBook();
			ExportExcel msbExcelExport(bookMSB);
			msbExcelExport.exportPostPreEncryptionToExcel(0, 2, "withMSB", beforeMSB, decryption.Image.clone());
			bookMSB->save(msbExcelExport.convertStringToTCHAR(imageName[imageIndex]));
			bookMSB->save(L"EncryptDecryptMSB.xls");
			bookMSB->release();

		}*/

	}


	return 0;

}
