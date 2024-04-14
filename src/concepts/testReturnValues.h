#pragma once

namespace TestReturnMultipleValues {
	// 1. std::pair pa; pa.first, pa.second

	// 2. std::tuple tu; std::get<0>(tu), std::get<1>(tu)
	// both of them are not clear, cause no one knows what means index, 0, 1

	// 3. with struct, this one is best
	/*
		stuct ShaderProgramSource{
			std::string VertexSource;
			std::string FragmentSource;
		}
	*/

	// put references or pointers into function, then the ouput variables in parent function needs to be pre allocated.

}

void test() {


}