#include "hashed_video.hpp"
#include <string>
#include <unistd.h>
#include <pwd.h>

std::string videoFolder() {
    std::string homeDir = getenv("HOME")
        ? getenv("HOME") 
        : getpwuid(getuid())->pw_dir;
    return homeDir + "/.videos";
}

ContentID::HashedVideo::HashedVideo(std::string videoid) {
    this->videoid = videoid;
    const std::string vf = videoFolder();
    std::ifstream binExists(vf + "/" + this->videoid + ".asset");
    std::ifstream mp4Exists(vf + "/" + this->videoid + ".mp4"); 
    if (!binExists) {
        if(!mp4Exists) {
            const std::string command("youtube-dl -f 'bestvideo[height<=144][ext=mp4]' -o '" + vf + "/%(id)s.%(ext)s' " + this->videoid);
            if(VERBOSE)
            std::cout << command << "\n";
            std::system(command.c_str());
        }
        cv::VideoCapture cap(vf + "/" + videoid + ".mp4");
        cv::Mat frame;
        this->framerate = cap.get(cv::CAP_PROP_FPS);
        this->videoid = videoid;
        while (cap.read(frame)) {
            HashedFrame hash(frame);
            this->frames.push_back(hash);
        }

        //const float f = 3.14f;
        //std::ofstream ofile(vf+"/"+this->videoid+"/.asset", std::ios::binary);
        //ofile.write((char*) &f, sizeof(float));
    }
}