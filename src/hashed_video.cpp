#include "hashed_video.hpp"
#include <string>
#include <unistd.h>
#include <pwd.h>
#include <stdexcept>

namespace {
    std::string video_directory() {
        std::string home_directory = getenv("HOME")
            ? getenv("HOME") 
            : getpwuid(getuid())->pw_dir;
        return home_directory + "/.videos";
    }

    bool file_exists(const std::string &path) {
        std::ifstream exists{path};
        return (bool) exists;
    }

    void download_video(const std::string &directory, const std::string &videoid) {
        const std::string command{"youtube-dl -f 'bestvideo[height<=144][ext=mp4]' -o '" + directory + "/%(id)s.%(ext)s' " + videoid};
        if (VERBOSE) std::cout << command << "\n";
        std::system(command.c_str());
    }

    std::string download_video_if_not_exists(const std::string &directory, const std::string &videoid) {
        const std::string video_path = directory + "/" + videoid + ".mp4";
        
        if (!file_exists(video_path))
            download_video(directory, videoid);
        
        if (!file_exists(video_path)) 
            throw std::runtime_error{"Unable to download video: " + videoid};
        
        return video_path;
    }
}


ContentID::HashedVideo::HashedVideo(std::string videoid) : videoid{videoid} {
    const std::string directory = ::video_directory();
    std::string path = ::download_video_if_not_exists(directory, videoid);
    cv::VideoCapture cap{path};
    cv::Mat frame;
    this->framerate = cap.get(cv::CAP_PROP_FPS);
    while (cap.read(frame))
        this->frames.push_back(ContentID::HashedFrame{frame});
}