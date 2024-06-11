#pragma once
#include <memory>
#include <iostream>
#include <string>

namespace TestAdapterDesignPattern{
/*
Adapter Pattern (Adapter Pattern) is a structural design pattern that allows you to convert the 
interface of one class to another interface so that these classes that would not otherwise work together due to interface incompatibility can work together.

Below is a C++ example that uses the Adapter Pattern. In this example, we will use the adapter 
pattern to adapt an existing class to a new interface.

*/
	class OldAudioPlayer {
	public:
		void play(const std::string& filename){
			std::cout << "Playing MP3 file: " << filename << "\n";
		}

	};

	class AudioPlayer {
	public:
	virtual void play(const std::string& filename) = 0;

	};

}

void test(){


}