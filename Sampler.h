#ifndef SAMPLER_H
#define SAMPLER_H

#include "ofMain.h"
#include "AudioFile.h"

class Sampler {
public:

    Sampler(){};

    void setup(string fileName="sounds/kick.wav"){

        name = fileName;
//        ofLog(OF_LOG_NOTICE, name);
        audioFile.load ("../bin/data/sounds/"+fileName+".wav");
//        audioFile.printSummary();
//        ofLog(OF_LOG_NOTICE, "");
        sampleRate = audioFile.getSampleRate();
        numSamples = audioFile.getNumSamplesPerChannel();
        numChannels = audioFile.getNumChannels();

        params["end"] = numSamples;
        params["start"] = 0.0;
        params["len"] = 1.0;
        params["active"] = false;
        params["volume"] = 1.0;
        params["pan"] = 0.5;
        params["looping"] = false;
        params["pitch"] = 1.0;
    };

    void process(ofSoundBuffer &output){

        if ( params.at("active") ){

            float volume = params.at("volume");
            float pan = params.at("pan");

            for (int i = 0; i < (int) output.size(); i += 2 )
            {
                output[i] += audioFile.samples[0][index] * volume * (1-pan);
                output[i+1] += audioFile.samples[ numChannels > 1 ? 1 : 0 ][index] * volume * (pan);


                if (index >= params["end"] ){
                    index = params["start"];
                    if ( !params["looping"] )
                        params["active"] = false;
                }
                else if (index < params["start"]){
                    index = params["end"]-1;
                    if ( !params["looping"] )
                        params["active"] = false;
                }
                else {
                    index += params["pitch"];
                }

            }
        }

    };

    void trigger(bool interrupt=true){
        if (params["looping"]){
            params["active"] = true;
        }
        else {
            if (interrupt){
                params["active"] = true;
                if (params["pitch"] > 0){
                    index = params["start"];
                } else {
                    index = params["end"]-1;
                }
            } else {
                params["active"] = true;
            }
        }
    }

    void setParam(string param_name, float value){
        if (params.count(param_name)>0){
            if (param_name == "len"){
                setLen(value);
            }
            else if (param_name == "start"){
                setStart(value);
            }
            else {
                params.at(param_name) = value;
            }
        }
    }

    void setStart(float new_start){
        int start_sample = (int) (numSamples-1)*new_start;
        params["start"] = start_sample <= numSamples-1 ? start_sample : numSamples-1;
    }
    void setLen(float new_len){
        params["len"] = new_len;
        float end = params["start"]+new_len*( numSamples-1-params["start"] );
        params["end"] = end <= numSamples-1 ? end : numSamples-1;
    }

    void draw_graph(float x, float y, float width, float height){

        ofSetColor(255);

        ofPolyline p;

        for (int samp = 0; samp < numSamples; ++samp) {
            ofVec3f vec( x+samp*(width/numSamples), y+audioFile.samples[0][samp]*height/2,0.0);
            p.addVertex(vec);
        }
        p.draw();
    }

    void draw_info(float x, float y, float width, float height){
        float xUnit = width/5;

        ofSetColor(255);
        ofDrawBitmapString(name, x,y);
        ofDrawBitmapString(params["start"], x+xUnit,y);
        ofDrawBitmapString(params["end"], x+xUnit*2,y);
        ofDrawBitmapString(params["pan"], x+xUnit*3,y);
        ofDrawBitmapString(params["volume"], x+xUnit*4,y);
    }

private:
    AudioFile<float> audioFile;

    int sampleRate = 44100;
    int numSamples = 0;
    int numChannels = 0;
    string name = "";
    float index = 0;

    std::map<string, float> params;
};

#endif // SAMPLER_H
