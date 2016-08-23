
#include"Utility.h"

RegisterFunMap fun_map;

namespace TK
{
	bool tk_is_file_existed(const char* filename)
	{
		if (_access(filename, 0) != -1)
		{
			std::cout << "File " << filename << " already exists.\n" << std::endl;
			return true;
		}

		return false;
	}

	template<class T>
	bool tk_normalize(T** data, int Y, int X)
	{
		T max_value = -1;
		T min_value = 100000000;
		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				max_value = std::max(max_value, data[y][x]);
				min_value = std::min(min_value, data[y][x]);
			}

		}

		assert(max_value > 0);
		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				data[y][x] = (data[y][x] / max_value) * 255;
			}

		}

		printf("Normalize Done,max = %f , min = %f\n", max_value, min_value);
		return true;
	}


	template<class T>
	bool tk_check(T** data, int Y, int X)
	{

		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				if (data[y][x] < 0)

					printf("data[%d][%d] = %f \n", y, x, data[y][x]);
			}

		}

		printf("check Done\n");
		return true;
	}

	template<class T>
	bool tk_memset(T** data, int Y, int X)
	{

		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				data[y][x] = 0;
			}

		}

		printf("memset  Done\n");
		return true;
	}


	bool tk_save_img(const cv::Mat img, const char* filename)
	{
		if (!img.data)
		{
			printf("The image to be saved is empty.\n");
			return false;
		}

		cv::imwrite(filename, img);

		return true;
	}

	template<class T>
	bool tk_dump_vec(const std::vector<std::vector<T>> data, int Y, int X, const char* filename)
	{
		std::fstream out(filename, std::ios::out);

		if (!out)
		{
			std::cout << "Open File Error" << std::endl;
			return false;
		}

		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				out << data[y][x] << " ";
			}
			out << std::endl;
		}
		out.close();

		printf("dump file Done --->%s\n", filename);
		return true;
	}

	template<class T>
	bool tk_elicit_vec(std::vector<std::vector<T>> & data, int Y, int X, const char* filename)
	{
		std::fstream in(filename, std::ios::in);

		if (!in)
		{
			std::cout << "Open File Error" << std::endl;
			return false;
		}

		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				in >> data[y][x];
			}

		}
		in.close();

		printf("get file Done --->%s\n", filename);
		return true;
	}


	bool tk_dump_points(const std::vector<std::vector<cv::Point>> data, const char* filename)
	{
		std::fstream out(filename, std::ios::out);

		if (!out)
		{
			std::cout << "Open File Error" << std::endl;
			return false;
		}

		for (int i = 0; i < data.size(); ++i)
		{
			out << data[i].size() << std::endl;

			for (int j = 0; j < data[i].size(); ++j)
			{
				out << data[i][j].x << " " << data[i][j].y << std::endl;
			}

		}

		out.close();

		printf("dump points Done --->%s\n", filename);

		return true;
	}


	bool tk_elicit_points(std::vector<std::vector<cv::Point>> & data, const char* filename)
	{
		std::fstream in(filename, std::ios::in);

		if (!in)
		{
			std::cout << "Open File Error" << std::endl;
			return false;
		}

		int num = 0;

		while (in >> num)
		{
			assert(num);
			std::vector<cv::Point> slicPoints;
			slicPoints.resize(num);
			for (int i = 0; i < num; ++i)
			{
				in >> slicPoints[i].x >> slicPoints[i].y;
			}
			data.push_back(slicPoints);

		}

		in.close();

		printf("get points Done --->%s\n", filename);

		return true;
	}

	template<class T>
	bool tk_dump_malloc(T** data, int Y, int X, const char* filename)
	{
		std::fstream out(filename, std::ios::out);

		if (!out)
		{
			std::cout << "Open File Error" << std::endl;
			return false;
		}

		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				out << data[y][x] << " ";
			}
			out << std::endl;
		}
		out.close();

		printf("dump file Done --->%s\n", filename);
		return true;
	}

	template<class T>	
	bool tk_elicit_malloc(T** & data, int Y, int X, const char* filename)
	{
		std::fstream in(filename, std::ios::in);

		if (!in)
		{
			std::cout << "Open File Error" << std::endl;
			return false;
		}

		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				in >> data[y][x];
			}

		}
		in.close();

		printf("get file Done --->%s\n", filename);
		return true;
	}

	template<>
	bool tk_elicit_malloc(double** & data, int Y, int X, const char* filename)
	{
		std::fstream in(filename, std::ios::in);

		if (!in)
		{
			std::cout << "Open File Error" << std::endl;
			return false;
		}

		for (int y = 0; y < Y; ++y)
		{
			for (int x = 0; x < X; ++x)
			{
				in >> data[y][x];
			}

		}
		in.close();

		printf("get file Done --->%s\n", filename);
		return true;
	}



	template<class T>
	std::string tk_toString(const T& t)
	{
		std::ostringstream oss;//����һ����


		oss.clear();

		oss << t;//��ֵ����������

		return oss.str();//��ȡת������ַ�ת������д��result

	}

	const char* cublasGetErrorString(cublasStatus_t error) {
		 switch (error) {
		 case CUBLAS_STATUS_SUCCESS:
			 return "CUBLAS_STATUS_SUCCESS";
		 case CUBLAS_STATUS_NOT_INITIALIZED:
			 return "CUBLAS_STATUS_NOT_INITIALIZED";
		 case CUBLAS_STATUS_ALLOC_FAILED:
			 return "CUBLAS_STATUS_ALLOC_FAILED";
		 case CUBLAS_STATUS_INVALID_VALUE:
			 return "CUBLAS_STATUS_INVALID_VALUE";
		 case CUBLAS_STATUS_ARCH_MISMATCH:
			 return "CUBLAS_STATUS_ARCH_MISMATCH";
		 case CUBLAS_STATUS_MAPPING_ERROR:
			 return "CUBLAS_STATUS_MAPPING_ERROR";
		 case CUBLAS_STATUS_EXECUTION_FAILED:
			 return "CUBLAS_STATUS_EXECUTION_FAILED";
		 case CUBLAS_STATUS_INTERNAL_ERROR:
			 return "CUBLAS_STATUS_INTERNAL_ERROR";
#if CUDA_VERSION >= 6000
		 case CUBLAS_STATUS_NOT_SUPPORTED:
			 return "CUBLAS_STATUS_NOT_SUPPORTED";
#endif
#if CUDA_VERSION >= 6050
		 case CUBLAS_STATUS_LICENSE_ERROR:
			 return "CUBLAS_STATUS_LICENSE_ERROR";
#endif
		 }
		 return "Unknown cublas status";
	 }

	const char* curandGetErrorString(curandStatus_t error) {
		 switch (error) {
		 case CURAND_STATUS_SUCCESS:
			 return "CURAND_STATUS_SUCCESS";
		 case CURAND_STATUS_VERSION_MISMATCH:
			 return "CURAND_STATUS_VERSION_MISMATCH";
		 case CURAND_STATUS_NOT_INITIALIZED:
			 return "CURAND_STATUS_NOT_INITIALIZED";
		 case CURAND_STATUS_ALLOCATION_FAILED:
			 return "CURAND_STATUS_ALLOCATION_FAILED";
		 case CURAND_STATUS_TYPE_ERROR:
			 return "CURAND_STATUS_TYPE_ERROR";
		 case CURAND_STATUS_OUT_OF_RANGE:
			 return "CURAND_STATUS_OUT_OF_RANGE";
		 case CURAND_STATUS_LENGTH_NOT_MULTIPLE:
			 return "CURAND_STATUS_LENGTH_NOT_MULTIPLE";
		 case CURAND_STATUS_DOUBLE_PRECISION_REQUIRED:
			 return "CURAND_STATUS_DOUBLE_PRECISION_REQUIRED";
		 case CURAND_STATUS_LAUNCH_FAILURE:
			 return "CURAND_STATUS_LAUNCH_FAILURE";
		 case CURAND_STATUS_PREEXISTING_FAILURE:
			 return "CURAND_STATUS_PREEXISTING_FAILURE";
		 case CURAND_STATUS_INITIALIZATION_FAILED:
			 return "CURAND_STATUS_INITIALIZATION_FAILED";
		 case CURAND_STATUS_ARCH_MISMATCH:
			 return "CURAND_STATUS_ARCH_MISMATCH";
		 case CURAND_STATUS_INTERNAL_ERROR:
			 return "CURAND_STATUS_INTERNAL_ERROR";
		 }
		 return "Unknown curand status";
	 }


}