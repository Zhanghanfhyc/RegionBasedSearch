#include "stdafx.h"
#include "WatershedAlgorithm.h"


void WatershedAlgorithm::run(IplImage* pSrc, const std::string& imgName)//分水岭核心算法
{
	std::string inTmp;

	//gray image
	IplImage* pGray = cvCreateImage(cvGetSize(pSrc), IPL_DEPTH_8U, 1);
	if (pSrc->nChannels == 3) {
		cvCvtColor(pSrc, pGray, CV_BGR2GRAY);
		inTmp = imgName + "_Gray.jpg";cvSaveImage(inTmp.c_str(), pGray);
	}
	else if (pSrc->nChannels == 1) {
		cvCopy(pSrc, pGray);
	}
	//Gaussian smooth
	IplImage* pSmooth = cvCreateImage(cvGetSize(pSrc), IPL_DEPTH_8U, 1);
	cvSmooth(pGray, pSmooth, CV_GAUSSIAN, 3, 3);
	//generate grad image
	IplImage* gradImagex = cvCreateImage(cvGetSize(pGray), pGray->depth, 1);
	IplImage* gradImagey = cvCreateImage(cvGetSize(pGray), pGray->depth, 1);
	IplImage* gradImage = cvCreateImage(cvGetSize(pGray), pGray->depth, 1);

	cvSobel(pSmooth, gradImagex, 0, 2, 3);
	cvSobel(pSmooth, gradImagey, 2, 0, 3);
	cvAdd(gradImagex, gradImagey, gradImage);


	labelMat = cvCreateImage(cvGetSize(pSrc), IPL_DEPTH_16U, 1);
	unsigned short* plabelMat = (unsigned short*)labelMat->imageData;

	
	char* pixels = gradImage->imageData;
	int width = gradImage->width;
	int heigh = gradImage->height;

	//=======================watershed algorethm===========================================================
	WatershedStructure watershedStructure(pixels, width, heigh);

	//flooding
	std::queue<WatershedPixel*> pque;	

	int heightIndex1 = 0;//current layer
	int heightIndex2 = 0;

	for (int h = HMIN; h < HMAX; ++h) { // layerh-1  Geodesic SKIZ
		for (int pixelIndex = heightIndex1;pixelIndex < watershedStructure.size();++pixelIndex) {
			WatershedPixel*p = watershedStructure.at(pixelIndex);

			if (p->getIntHeight() != h) {//p=h+1，end of h
				heightIndex1 = pixelIndex;
				break;
			}

			p->setLabelToMASK();

			std::vector<WatershedPixel*> neighbours = p->getNeighbours();
			for (unsigned i = 0;i < neighbours.size();++i) {
				WatershedPixel* pnei = neighbours.at(i);
				
				if (pnei->getLabel() > 0) {
					p->setDistance(1);pque.push(p);break;
				}
			}
		}

		int curDist = 1;
		pque.push(new WatershedPixel());

		//extent basin
		while (true) {
			WatershedPixel* p = pque.front();pque.pop();

			if (p->isFICTITIOUS()) {
				if (pque.empty()) { delete p;p = NULL;break; }
				else {
					pque.push(new WatershedPixel());
					curDist++;
					delete p;p = pque.front();pque.pop();
				}
			}

			std::vector<WatershedPixel*> neighbours = p->getNeighbours();
			for (unsigned i = 0;i < neighbours.size();++i) {//通过检查邻接像素来标记p
				WatershedPixel* pnei = neighbours.at(i);

				//nei属于一个存在的盆地或分水线
				if (pnei->getDistance() < curDist&&pnei->getLabel() >= 0) {
					if (pnei->getLabel() > 0) {//如果当前存在邻接像素已经被标记了
						if (p->isLabelMASK())
							p->setLabel(pnei->getLabel());
						else if (p->getLabel() != pnei->getLabel())//如果p旁边还有别的集水盆
							p->setLabelToWSHED();
					}
					else if (p->isLabelMASK())//？？？
						p->setLabelToWSHED();
				}
				else if (pnei->isLabelMASK() && pnei->getDistance() == 0) {
					pnei->setDistance(curDist + 1);
					pque.push(pnei);
				}
			}//处理邻接像素结束
		}//拓展集水盆结束

		 //重新扫描还没有属于任何一个集水盆的区域
		for (int pixelIndex = heightIndex2; pixelIndex < watershedStructure.size(); pixelIndex++) {
			WatershedPixel* p = watershedStructure.at(pixelIndex);

			/* 此像素点位于 h+1 层，暂不处理，跳出循环 */
			if (p->getIntHeight() != h) { heightIndex2 = pixelIndex; break; }

			p->setDistance(0); // 重置距离为0

			if (p->isLabelMASK()) { // 该像素位于新最小值区域
				curlab++;
				p->setLabel(curlab);
				pque.push(p);

				while (!pque.empty()) {
					WatershedPixel* q = pque.front();
					pque.pop();

					std::vector<WatershedPixel*> neighbours = q->getNeighbours();

					for (unsigned i = 0; i < neighbours.size(); i++) { // 检查p2的邻域像素
						WatershedPixel* r = neighbours.at(i);

						if (r->isLabelMASK()) { r->setLabel(curlab); pque.push(r); }
					}
				} // end while
			} // end if
		} // end for
	}



	/************************ flooding end ****************************/

	/* generate watershed image */
	pWS = cvCreateImage(cvGetSize(pSrc), IPL_DEPTH_8U, 3);
	IplImage* pWS_g = cvCreateImage(cvGetSize(pSrc), IPL_DEPTH_8U, 1);
	int channels = pWS->nChannels;
	int step = pWS->widthStep;

	char* wsPixels = pWS->imageData;
	char* wsPixels_g = pWS_g->imageData;
	char* grayPixels = pGray->imageData;
	std::cout << "lable num: " << curlab;
	for (int i = 0;i<pWS->height;i++)
		for (int j = 0;j<pWS->width;j++) {

			wsPixels[i*step + j * 3] = 0;
			wsPixels[i*step + j * 3 + 1] = 0;
			wsPixels[i*step + j * 3 + 2] = 0;

			wsPixels_g[i*width + j] = 0;




		}

	for (int pixelIndex = 0; pixelIndex < watershedStructure.size(); pixelIndex++) {
		WatershedPixel* p = watershedStructure.at(pixelIndex);

		if (p->isLabelWSHED()) {
			wsPixels[p->getY()*pWS->width * 3 + p->getX()*pWS->nChannels] = 255;
			wsPixels[p->getY()*pWS->width * 3 + p->getX() *pWS->nChannels + 1] = 255;
			wsPixels[p->getY()*pWS->width * 3 + p->getX() *pWS->nChannels + 2] = 255;

			wsPixels_g[p->getX() + p->getY()*pWS_g->width] = (char)255;
			grayPixels[p->getX() + p->getY()*width] = (char)255;	
		}
		plabelMat[p->getY()*labelMat->width + p->getX()] = p->getLabel();
	}



	inTmp = imgName + "_WS.jpg"; cvSaveImage(inTmp.c_str(), pWS);
	inTmp = imgName + "_Gray_WS.jpg"; cvSaveImage(inTmp.c_str(), pGray);


	cvReleaseImage(&pGray);

}