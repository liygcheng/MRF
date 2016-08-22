
#include<fstream>
#include<map>
#include<gflags\gflags.h>
#include<glog\logging.h>

#include"GraphCut4MRF.h"


DEFINE_string(infolder,"./","Optional: root folder , default setting is current workspace.");
DEFINE_string(gpu, "",
	"Optional; run in GPU mode on given device IDs separated by ','."
	"Use '-gpu all' to run on all available GPUs. The effective training "
	"batch size is multiplied by the number of devices.");

DEFINE_string(imageName,"","image path for all the testing usage.");
DEFINE_bool(dumpImage,false,"dump current image to the folder of imageName,rename output as dumpName");
DEFINE_string(dumpName, "", "dump image name after processing of imageName");

DEFINE_bool(showEdge, false, "display edges of source image.");
DEFINE_string(edgeSolver, "canny", "edge detection using different kind of methods");


DEFINE_bool(showInitialImage, false, "show initial image of synthesis"); 
DEFINE_bool(dumpInitialImage,false,"dump initial image of synthesis");

typedef int(*RegisterFunction)(void);
typedef std::map<std::string, RegisterFunction> RegisterFunMap;
RegisterFunMap fun_map;

#define doRegisteration(fun)	\
namespace{						\
class _Register_##fun{			\
public:	_Register_##fun()		\
	{							\
fun_map[#fun] = &(fun);			\
    }							\
};							\
_Register_##fun m_registeration_##fun;\
}							\

static RegisterFunction  getCommandFunction(const std::string& name)
{
	if (fun_map.count(name)){
		return fun_map[name];
	}
	else
	{
		LOG(ERROR) << "Available Actions:";
		for (RegisterFunMap::iterator it = fun_map.begin(); it != fun_map.end(); ++it)
		{
			LOG(ERROR) << "\t" << it->first;
		}
		LOG(FATAL) << "unknown actions :" << name;
		return NULL;

	}

}

// 
// 1.  edge detection ,sobel,canny,laplacian operator
int edgeDetection(void)
{
	CHECK_GT(FLAGS_imageName.size(), 0) << " image to be detected should not be empty..";


	cv::Mat m = cv::imread(FLAGS_imageName, CV_LOAD_IMAGE_UNCHANGED);

	cv::Mat gray, result;

	if (!m.data)
	{
		std::cout << "read image error,aborting.." << std::endl;
		return -1;
	}

	if (FLAGS_edgeSolver == "canny")
	{
		cv::Canny(m, result, 50, 150,3);
	}
	else if (FLAGS_edgeSolver == "sobel")
	{
		cv::Sobel(m, result, m.depth(), 1, 1);
	}
	else if (FLAGS_edgeSolver == "laplacian")
	{
		cv::Laplacian(m,result,m.depth());
	}
	else
	{
		LOG(ERROR) << " No other edge detection method.";
	}

	

	if (FLAGS_dumpImage)
	{
		CHECK_GT(FLAGS_dumpName.size(), 0) << "dump name should not be empty.";




	}
	return  0;
}

doRegisteration(edgeDetection);


//

void GetMatArray(const char* filename,MatArray& output)
{
	std::string matfile;
	std::ifstream in(filename);

	while (in>>matfile)
	{
		cv::Mat m = cv::imread(matfile, CV_LOAD_IMAGE_UNCHANGED);

		if (!m.data)
		{
			std::cout << "read image error,aborting.."<<std::endl;
			break;
		}

		output.push_back(m);
	}

	in.close();
}

int main(int argc,char** argv)
{
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	//std::cout << FLAGS_infolder << std::endl;

	//MatArray m_source, m_slic, m_dist,m_pointMask;
	
	//GetMatArray("./data_H1_1/source.txt", m_source);

	//GetMatArray("./data_H1_1/labels.txt", m_slic);

	//GetMatArray("./data_H1_1/distField.txt", m_dist);

	//GetMatArray("./data_H1_1/pointMask.txt", m_pointMask);



	//GraphCut4MRF* m_GraphCut = new GraphCut4MRF(m_source, m_slic, m_dist,m_pointMask);

	//m_GraphCut->edgeDetect();

	//m_GraphCut->initGraph("./data_H1_1/InitLabel.png");
	//m_GraphCut->saveMask("./subtotal/InitMask.png");

	//m_GraphCut->graphCutOptimization(1, "");
 //      
	//m_GraphCut->showResult();

	//m_GraphCut->saveMask("./subtotal/mask_patch.png");

	//TK::tk_save_img(m_GraphCut->GetResult());

	//system("pause");




	return 0;
}