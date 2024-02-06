#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#include <fstream>

struct BinaryBall {

	struct Node {
		std::string Path;
		std::vector <std::uint8_t> Data;
	};

	std::vector<Node> Nodes;


	bool Add(const Node& N) {
		Nodes.push_back(N);
		return true;
	}
	inline static const char* Sig = "Ball  \n\0";//space is reserved. // need c++17
	struct IndexRecode {
		std::uint64_t DataPositin = 0;
		std::uint64_t PathLengrh = 0;
		std::string Path;
	};
	struct DataRecode {
		std::uint64_t DataSize = 0;
		std::vector<std::uint8_t> Data;
	};
	/**/
	struct Ball {

		std::vector<IndexRecode> Index;
		std::vector<DataRecode> Data;

	};
	/**/
	BinaryBall::Ball ConstructStruct() {

		const BinaryBall& In = *this;
		BinaryBall::Ball B;

		std::uint64_t AP = 0;

		for (std::size_t i = 0; i < In.Nodes.size(); i++) {
			BinaryBall::IndexRecode IR;
			BinaryBall::DataRecode DR;

			IR.DataPositin = sizeof(std::uint64_t) * i + AP;
			IR.PathLengrh = In.Nodes[i].Path.size();
			IR.Path = In.Nodes[i].Path;
			B.Index.push_back(IR);
			DR.DataSize = In.Nodes[i].Data.size();
			AP += In.Nodes[i].Data.size();
			DR.Data.insert(DR.Data.end(), In.Nodes[i].Data.begin(), In.Nodes[i].Data.end());
			B.Data.push_back(DR);
		}

		return B;
	}
};

struct Data {
	std::vector<std::uint8_t> Index;
	std::vector<std::uint8_t> Data;
};

union u64 {
	std::uint64_t Single= 0;
	std::uint8_t Multi[8];
};


Data ConstructData(const BinaryBall::Ball& In) {

	u64 X;
	std::vector<std::uint8_t> D;
	for (auto o : In.Index) {
		X.Single = o.DataPositin;
		D.insert(D.end(), X.Multi, X.Multi + 8);
		X.Single = o.PathLengrh;
		D.insert(D.end(), X.Multi, X.Multi + 8);
		D.insert(D.end(), o.Path.begin(),o.Path.end());
	}
	std::vector<std::uint8_t> V;
	for (auto o : In.Data) {
		X.Single = o.DataSize;
		V.insert(V.end(), X.Multi, X.Multi + 8);
		V.insert(V.end(), o.Data.begin(),o.Data.end());
	}
	return { D,V };
}
int main() {

	BinaryBall BB;
	BB.Add({ "c:/x/y/z/HOgeratta.bmp",{1,2,3,4,5,6,7} });
	BB.Add({ "c:/x/y/z/a/HAgeratta.bmp",{1,2,3,4,5,6,7} });
	BB.Add({ "c:/x/y/merigeratta.bmp",{1,2,3,4,5,6,7} });

	auto B = BB.ConstructStruct();

	auto X = ConstructData(B);

	std::ofstream ofs("Con.data", std::ios::binary);

	std::vector<char> R;

	R.insert(R.end(), X.Index.begin(), X.Index.end());
	R.insert(R.end(), X.Data.begin(), X.Data.end());

	ofs.write( &R[0], R.size());
	return 0;
}