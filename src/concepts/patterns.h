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
		virtual void play(const std::string& audioType, const std::string& filename) = 0;

	};

	class AudioPlayerAdapter : public AudioPlayer{
	public:
		AudioPlayerAdapter(OldAudioPlayer* oldplayer) : _oldPlayer(oldplayer){}

		void play(const std::string& audioType, const std::string& filename){
			if (audioType == "mp3"){
				_oldPlayer->play(filename);
			}else{
				std::cout << "Unsupported audio type: " << audioType << ", filename: " << filename << "\n";
			}
		
		}

	private:
		OldAudioPlayer* _oldPlayer;

	};

	void test(){
		OldAudioPlayer* oldPlayer = new OldAudioPlayer();
		AudioPlayer* player = new AudioPlayerAdapter(oldPlayer);

		player->play("mp3", "song.mp3");
		player->play("wav", "song.wav");

		delete player;
		delete oldPlayer;
	
	}

}

void test(){
	TestAdapterDesignPattern::test();

}