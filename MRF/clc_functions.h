#pragma once

#include"Utility.h"

#include"clc_mouse.h"

#include"cuda_sift.cuh"

RegisterFunMap fun_map;


extern int edge_detection(void);
doRegisteration(edge_detection);
extern int device_query(void);
doRegisteration(device_query);
extern int graph_cut(void);
doRegisteration(graph_cut);
extern int slic(void);
doRegisteration(slic);
extern int extract_feature(void);
doRegisteration(extract_feature);
extern int blending(void);
doRegisteration(blending);

extern int scribble(void);
doRegisteration(scribble);

extern int test(void);
doRegisteration(test);

extern int computeSift(void);
doRegisteration(computeSift);

extern int jpg2pgm(void);
doRegisteration(jpg2pgm);


// 1.  edge detection ,sobel,canny,laplacian operator
int edge_detection(void)
{


	CHECK_GT(FLAGS_imageName.size(), 0) << " image to be detected should not be empty..";
	CHECK_GT(FLAGS_dumpName.size(), 0) << "dump name should not be empty.";

	cv::Mat m = cv::imread(FLAGS_imageName, CV_LOAD_IMAGE_UNCHANGED);

	cv::Mat gray, result;

	if (!m.data)
	{
		std::cout << "read image error,aborting.." << std::endl;
		return -1;
	}

	if (FLAGS_edgeSolver == "canny")
		cv::Canny(m, result, 50, 150, 3);
	else if (FLAGS_edgeSolver == "sobel")
		cv::Sobel(m, result, m.depth(), 1, 1);
	else if (FLAGS_edgeSolver == "laplacian")
		cv::Laplacian(m, result, m.depth());
	else
		LOG(ERROR) << " No other edge detection method.";



		cv::imshow("edge detection.", result);
		cv::waitKey(0);
	

		

		cv::imwrite(FLAGS_dumpName, result);
	
	return  1;
}


int device_query(void)
{
	LOG(INFO) << "Queryings GPUS " << FLAGS_gpu;

	std::vector<int> gpus;

	CUDA::get_gpus(&gpus);

	cudaDeviceProp prop;

	for (int i = 0; i < gpus.size(); ++i) {
		CUDA_CHECK(cudaGetDeviceProperties(&prop, i));
		printf("------General Information for GPU %d ---\n", i);
		printf("Name : %s\n", prop.name);
		printf("Compute capability: %d.%d\n", prop.major, prop.minor);
		printf("Clock rate: %d\n", prop.clockRate);
		printf("Device Copy Overlap: ");
		if (prop.deviceOverlap) printf("Enabled\n");
		else printf("Disabled\n");
		printf("Kernel Execution timeout: ");
		if (prop.kernelExecTimeoutEnabled) printf("Enabled\n");
		else printf("Disabled\n");

		printf("------Memory Information for GPU %d ---\n", i);
		printf("Total global memory: %ld\n", prop.totalGlobalMem);
		printf("Total constant memory: %ld\n", prop.totalConstMem);
		printf("Max Mem pitch:	%ld\n", prop.memPitch);
		printf("Texture Alignment: %ld\n", prop.textureAlignment);
		printf("Global memory bus width in bytes : %d \n", prop.memoryBusWidth);
		printf("Memory clock rate : %d \n", prop.memoryClockRate);

		printf("---MP information for device %d ----------\n", i);
		printf("Multiprocessor  count :   %d \n", prop.multiProcessorCount);
		printf("Shared mem per block : %ld \n", prop.sharedMemPerBlock);
		printf("Shared mem per Multiprocessor : %ld \n", prop.sharedMemPerMultiprocessor);
		printf("Registers  per block:  %d \n", prop.regsPerBlock);
		printf("Registers  per Multiprocessor:  %d \n", prop.regsPerMultiprocessor);
		printf("Threads in wrap :  %d  \n", prop.warpSize);
		printf("Max Threads per block:   %d \n", prop.maxThreadsPerBlock);
		printf("Max Thread dimension : (%d  ,  %d  ,  %d)\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
		printf("Max Grid dimension : (%d  ,  %d  ,  %d)\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
		printf("unique identifier  for gpu  multiGpuBoardGroupID: %d \n", prop.multiGpuBoardGroupID);
		printf("asynchronous engine count : %d \n", prop.asyncEngineCount);
		printf("Device can map host memory with cudaHostAlloc :  %d \n", prop.canMapHostMemory);
		
		printf("Compute Mode : ");
		switch (prop.computeMode)
		{
		case cudaComputeMode::cudaComputeModeDefault:
			printf("cudaComputeModeDefault\n");
			break;
		case cudaComputeMode::cudaComputeModeExclusive:
			printf("cudaComputeModeExclusive\n");
			break;
		case cudaComputeMode::cudaComputeModeExclusiveProcess:
			printf("cudaComputeModeExclusiveProcess\n");
			break;
		case cudaComputeMode::cudaComputeModeProhibited:
			printf("cudaComputeModeProhibited\n");
			break;
		default:
			printf("No other compute mode.\n");
			break;
		}

		printf("Device can possibly execute multiple kernels concurrently :  %d \n", prop.concurrentKernels);
		printf("Device has ECC support enabled :  %d \n", prop.ECCEnabled);
		printf("Device supports caching globals in L1 : %d \n", prop.globalL1CacheSupported);
		printf("is integrated : %d \n", prop.integrated);
		printf("Device is on a multi-GPU board : %d \n", prop.isMultiGpuBoard);
		printf("Size of L2 cache in bytes : %d \n", prop.l2CacheSize);
		printf("Device supports caching locals in L1 : %d \n", prop.localL1CacheSupported);
		printf("Device supports allocating managed memory on this system :  %d \n", prop.managedMemory);

		printf("------Surface Information for GPU %d ---\n", i);
		printf("Maximum 1D surface size : %d \n", prop.maxSurface1D);
		printf("Maximum 1D layered surface dimensions : %d  %d\n", prop.maxSurface1DLayered[0], prop.maxSurface1DLayered[1]);

		printf("Maximum 2D surface size : %d  %d \n", prop.maxSurface2D[0], prop.maxSurface2D[1]);
		printf("Maximum 2D layered surface dimensions : %d  %d  %d\n", prop.maxSurface2DLayered[0], prop.maxSurface2DLayered[1], prop.maxSurface2DLayered[2]);

		printf("Maximum 3D surface size : %d  %d  %d\n", prop.maxSurface3D[0], prop.maxSurface3D[1], prop.maxSurface3D[2]);

		printf("Maximum CubeMap surface dimension : %d \n", prop.maxSurfaceCubemap);
		printf("Maximum CubeMap layered surface dimension : %d  %d \n", prop.maxSurfaceCubemapLayered[0], prop.maxSurfaceCubemapLayered[1]);

		printf("------Texture Information for GPU %d ---\n", i);
		printf("Maximum 1D texture size : %d \n", prop.maxTexture1D);
		printf("Maximum 1D layered texture dimension : %d  %d \n", prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1]);
		printf("Maximum size for 1D texture bound to linear memory : %d \n", prop.maxTexture1DLinear);
		printf("Maximum 1D mipmapped texture size : %d \n", prop.maxTexture1DMipmap);

		printf("Maximum 2D texture dimensions : %d %d \n", prop.maxTexture2D[0], prop.maxTexture2D[1]);
		printf("Maximum 2D texture dimensions if texture gather operations to be performed : %d  %d \n", prop.maxTexture2DGather[0], prop.maxTexture2DGather[1]);
		printf("Maximum 2D layered texture dimensions : %d  %d  %d \n", prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);
		printf("Maximum dimensions (width  height pitch) for 2D textures bound to pitched memory : %d  %d  %d  \n", prop.maxTexture2DLinear[0], prop.maxTexture2DLinear[1], prop.maxTexture2DLinear[2]);
		printf("Maximum 2D mipmapped texture dimensions :  %d  %d  \n", prop.maxTexture2DMipmap[0], prop.maxTexture2DMipmap[1]);

		printf("Maximum 3D texture dimensions : %d  %d  %d \n", prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
		printf("Maximum 3D alternate texture dimensions : %d  %d  %d \n", prop.maxTexture3DAlt[0], prop.maxTexture3DAlt[1], prop.maxTexture3DAlt[2]);

		printf("Maximum cubemap texture dimensions : %d  \n", prop.maxTextureCubemap);
		printf("Maximnum cubemap layered dimensions : %d  %d \n", prop.maxTextureCubemapLayered[0], prop.maxTextureCubemapLayered[1]);

		printf("PCI   bus  ID  of Device :  %d \n", prop.pciBusID);
		printf("PCI   device  ID  of Device :  %d \n", prop.pciDeviceID);
		printf("PCI   domain  ID  of Device :  %d \n", prop.pciDomainID);
		printf("Device  support stream priority :  %d \n", prop.streamPrioritiesSupported);
		printf("Alignment requirement for surfaces :  %d \n", prop.surfaceAlignment);
		if (prop.tccDriver)
			printf("Device is a Tesla device  using TCC  driver \n");
		else
			printf("Other dirver, not (Tesla + TCC)\n");
		printf("Pitch alignment requirement for texture  reference bound to pitched memory :%d\n", prop.texturePitchAlignment);
		printf("Device shares a unified  address space with the host :  %d \n", prop.unifiedAddressing);





	}

	return 1;
}


int graph_cut(void)
{

	LOG(INFO) << "graph cut for markov random field\n";
	
	CHECK_GT(FLAGS_infolder.size(), 0) << " root  folder  (for source , slic ,dist and scribble points) should not be empty .\n";

	

	MatArray m_source, m_slic, m_dist,m_pointMask;

	std::string  infolder(FLAGS_infolder);

	std::string  subtotal;
	subtotal.assign("./subtotal");
	TK::tk_make_file(subtotal.c_str());
	
	TK::tk_get_mat_array(infolder + "/source.txt", m_source);
	TK::tk_get_mat_array(infolder + "/labels.txt", m_slic);
	TK::tk_get_mat_array(infolder + "/distField.txt", m_dist);
	TK::tk_get_mat_array(infolder + "/pointMask.txt", m_pointMask);


	GraphCut4MRF* m_GraphCut = new GraphCut4MRF(FLAGS_infolder,subtotal, m_source, m_slic, m_dist, m_pointMask);


	m_GraphCut->initGraph(infolder+"/InitLabel.png");

	m_GraphCut->saveMask(subtotal + "/InitMask.png");
	m_GraphCut->showResult(true,subtotal+"/InitResult.png");

	if (FLAGS_graphSolver == "alpha-expansion")
		m_GraphCut->graphCutOptimization(1, "", GraphCut4MRF::SolverMethod::Alpha_Expansion);
	else if (FLAGS_graphSolver == "alpha-beta-swap")
		m_GraphCut->graphCutOptimization(1, "", GraphCut4MRF::SolverMethod::Alpha_Beta_Swap);

	      
	

	m_GraphCut->saveMask(subtotal + "/mask_patch.png");

	TK::tk_save_img(m_GraphCut->GetResult(),subtotal+"/result.png");


	
	return 1;
}


int slic(void)
{
	printf("slic\n");
	return 1;
}


int extract_feature(void)
{
	printf("extract_feature\n");
	return 1;
}


int blending(void)
{
	CHECK_GT(FLAGS_infolder.size(),0)<<"input folder should not be empty..";

	if (!TK::tk_is_file_existed(FLAGS_infolder.c_str())) {

		std::cerr << "infolder does not exist." << std::endl;
		return -1;
	}
	  
	CHECK_GT(FLAGS_outfolder.size(), 0) << " dump folder  should not be empty..";

	////	//// check for exist
	if (!TK::tk_is_file_existed(FLAGS_outfolder.c_str()))
	{
		TK::tk_make_file(FLAGS_outfolder.c_str());
	}




	
	printf("blending");
	return 1;
}

int scribble(void){
//
//std::cout << "Hello " << std::endl;
CHECK_GT(FLAGS_imageName.size(), 0) << " -imageName should not be empty..";
CHECK_GT(FLAGS_outfolder.size(), 0) << " -outfolder folder  should not be empty..";

//	//// check for exist
	if (!TK::tk_is_file_existed(FLAGS_outfolder.c_str()))
	{
		TK::tk_make_file(FLAGS_outfolder.c_str());
	}

	MouseData* userData = new MouseData(FLAGS_imageName, FLAGS_outfolder);


	// Create a window for display.  
	cv::namedWindow("Input Image", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("Scribble Image", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("background mask", CV_WINDOW_AUTOSIZE);


	if (FLAGS_scribble_type == "line")
	cv::namedWindow("foreground mask", CV_WINDOW_AUTOSIZE);


	// Show our image inside it.  
	cv::imshow("Input Image", userData->m_source);
	cv::imshow("Scribble Image", userData->m_scribble);

	if (FLAGS_scribble_type == "line")
	cv::setMouseCallback("Scribble Image", onMouseScribble, (void*)userData);
	else if (FLAGS_scribble_type=="area")
	cv::setMouseCallback("Scribble Image", onMouseMatting, (void*)userData);

	

	while (true){

		char key = cv::waitKey(0);

		int flag = getMouseActionCommand(TK::tk_toString(key))(userData);
		
		if (flag == 0 || flag == -1) break;
		
		
	}   



	return 0;
}

int test(void)
{
	//test  for match
	if (FLAGS_imageName.size() && FLAGS_dumpName.size())
	{

		cv::Mat m1 = cv::imread(FLAGS_dumpName, 0);
		cv::Mat m2 = cv::imread(FLAGS_imageName, 0);

		m1.convertTo(m1, CV_32FC1);
		m2.convertTo(m2, CV_32FC1);

		//if (!m1.data || !m2.data)
		//{
		//	std::cout << "read image error,aborting.." << std::endl;
		//	continue;
		//}

		unsigned int w1 = m1.cols;
		unsigned int h1 = m1.rows;

		CudaImage m1_cudaImage;

		m1_cudaImage.Allocate(w1, h1, iAlignUp(w1, 128), false, NULL, (float*)m1.data);

		m1_cudaImage.Download();


		unsigned int w2 = m2.cols;
		unsigned int h2 = m2.rows;

		CudaImage m2_cudaImage;
		m2_cudaImage.Allocate(w2, h2, iAlignUp(w2, 128), false, NULL, (float*)m2.data);

		m2_cudaImage.Download();



		SiftData m1_siftData, m2_siftData;

		float thresh = 5.0f;

		InitSiftData(m1_siftData, 4096, true, true);
		InitSiftData(m2_siftData, 4096, true, true);


		ExtractSift(m1_siftData, m1_cudaImage, 5, 0.0f, thresh, 0.0f);
		ExtractSift(m2_siftData, m2_cudaImage, 5, 0.0f, thresh, 0.0f);


		MatchSiftData(m1_siftData, m2_siftData);

		//tk_write_Sift_Mat(&m1_siftData, "C:/Liygcheng/Research/TimeLapse/matlab/SFM/1.mat");
		//tk_write_Sift_Mat(&m2_siftData, "C:/Liygcheng/Research/TimeLapse/matlab/SFM/2.mat");


	

		printf("siftpoint1: Num match = %d\n", m1_siftData.h_data->match);
		printf("siftpoint2: Num match = %d\n", m2_siftData.h_data->match);
		FreeSiftData(m1_siftData);
		FreeSiftData(m2_siftData);

	}

	return 1;
}

int computeSift(void){

//#pragma omp parallel for

	CHECK_GT(FLAGS_infolder.size(), 0) << " image to be detected should not be empty..";
	CHECK_GT(FLAGS_outfolder.size(), 0) << " image to be detected should not be empty..";

	if (!TK::tk_is_file_existed(FLAGS_infolder.c_str())) {

		std::cerr << "infolder does not exist." << std::endl;
		return -1;
	}

	CHECK_GT(FLAGS_outfolder.size(), 0) << " dump folder  should not be empty..";

	////	//// check for exist
	if (!TK::tk_is_file_existed(FLAGS_outfolder.c_str()))
	{
		TK::tk_make_file(FLAGS_outfolder.c_str());
	}

	cv::Directory pgmroot;

	std::vector<std::string> filenames = pgmroot.GetListFiles(FLAGS_infolder, "*.pgm", false);

	std::cout << "Initializing data..." << std::endl;
	InitCuda(0);

	TimerGPU timer(0);

	for (int i = 0; i < filenames.size(); ++i){

		std::string pgmname = FLAGS_infolder + filenames[i];
		std::size_t pos = filenames[i].find("pgm");
		std::string matname = FLAGS_outfolder + filenames[i].substr(0, pos) + "mat";
		cv::Mat m = cv::imread(pgmname,0);
		m.convertTo(m, CV_32FC1);
		if (!m.data)
		{
			std::cout << "read image error,aborting.." << std::endl;
			continue;
		}
		//std::cout << "input file :------------> " << pgmname << std::endl;
		//std::cout << "output file :------------> " << matname << std::endl;


		unsigned int w = m.cols;
		unsigned int h = m.rows;

		CudaImage m_cudaImage;

		m_cudaImage.Allocate(w, h, iAlignUp(w, 128), false, NULL, (float*)m.data);

		m_cudaImage.Download();

		SiftData m_siftData;

		float thresh = 5.0f;

		InitSiftData(m_siftData, 4096, true, true);

		ExtractSift(m_siftData, m_cudaImage, 5, 0.0f, thresh, 0.0f);

		//printf("Number of available points: %d\n", m_siftData.numPts);

		if (m_siftData.numPts != 0){
			tk_write_Sift_Mat(&m_siftData, matname);
			FreeSiftData(m_siftData);
		}

	}

	double totTime = timer.read();
	printf("Total time incl memory =      %.2f ms\n", totTime);

	

	return 0;

}

 int jpg2pgm(void){

	 CHECK_GT(FLAGS_infolder.size(), 0) << "input folder should not be empty..";
	 CHECK_GT(FLAGS_outfolder.size(), 0) << " -outfolder folder  should not be empty..";
	 if (!TK::tk_is_file_existed(FLAGS_infolder.c_str())) {

		 std::cerr << "infolder does not exist." << std::endl;
		 return -1;
	 }

	 CHECK_GT(FLAGS_outfolder.size(), 0) << " dump folder  should not be empty..";

	 ////	//// check for exist
	 if (!TK::tk_is_file_existed(FLAGS_outfolder.c_str()))
	 {
		 TK::tk_make_file(FLAGS_outfolder.c_str());
	 }

	 cv::Directory jpgroot;

	 std::vector<std::string> filenames = jpgroot.GetListFiles(FLAGS_infolder, "*.jpg",false);


	 for each (std::string filename in filenames)
	 {
		 std::string jpgname = FLAGS_infolder + filename;
		 std::size_t pos = filename.find("jpg");
		 std::string pgmname = FLAGS_outfolder + filename.substr(0, pos) + "pgm";
		 cv::Mat im =  cv::imread(jpgname);

		 cv::imwrite(pgmname, im);
	 }





}