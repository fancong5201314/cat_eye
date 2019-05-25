#ifndef FACE_SERVICE_H_
#define FACE_SERVICE_H_

#include <list>

#include "camerahal.h"
#include "camerabuf.h"


class RKVideo {
 public:
    RKVideo();
    ~RKVideo();

    virtual void start(void);
    virtual void stop(void);
	int connect(std::shared_ptr<CamHwItf::PathBase> mpath, 
		            std::shared_ptr<StreamPUBase> next,
                    frm_info_t& frmFmt, const uint32_t num, 
                    std::shared_ptr<RKCameraBufferAllocator> allocator);
    int connect(std::shared_ptr<StreamPUBase> pre, 
		            std::shared_ptr<StreamPUBase> next,
                    frm_info_t &frmFmt, const uint32_t num, 
                    std::shared_ptr<RKCameraBufferAllocator> allocator);

    void disconnect(std::shared_ptr<CamHwItf::PathBase> mpath, 
		                 std::shared_ptr<StreamPUBase> next);
    void disconnect(std::shared_ptr<StreamPUBase> pre, 
		                 std::shared_ptr<StreamPUBase> next);

 private:
 	
    struct rk_cams_dev_info cam_info;
    std::shared_ptr<RKCameraBufferAllocator> ptr_allocator;
    CameraFactory cam_factory;
    std::list<shared_ptr<RKCameraHal>> camhals;
    std::shared_ptr<DisplayProcess> display_process;
};




#endif
