#linear_blur: srgb to linear, simple blur, linear to srgb
import heterocl as hcl 
import heterocl.tvm as tvm 
hcl.init()
#hcl.init(hcl.Float()) # if comment, output will be int. Don't know which step has problems

input = hcl.placeholder((768 * 1280 * 3, ), name = "input", dtype = hcl.Float()) #input shape = output shape, (w, h, c)

def linear_blur(input):
    def final(_srgb_s0_x, _srgb_s0_y, _srgb_s0_c, _input):
        out = hcl.local(0, dtype = hcl.Float())
        with hcl.if_ (tvm.all(_srgb_s0_y >= 0, _srgb_s0_y < 0)): # y for
            with hcl.if_ (tvm.all(_srgb_s0_x < 768, _srgb_s0_x >= 0)): # x for     tvm.all or two with loop? can't use and
                _t263 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765) > -2, hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765), -2) + (hcl.select(_srgb_s0_y > 0, _srgb_s0_y, 0) * 768))) + 2)]
                _t265 = _input[((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767) > 0, hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767), 0) + (hcl.select(_srgb_s0_y > 0, _srgb_s0_y, 0) * 768)))]
                _t267 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766) > -1, hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766), -1) + (hcl.select(_srgb_s0_y > 0, _srgb_s0_y, 0) * 768))) + 1)]
                _t269 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765) > -2, hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765), -2) + (hcl.select(_srgb_s0_y > -1, _srgb_s0_y, -1) * 768))) + 770)]
                _t270 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767) > 0, hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767), 0) + (hcl.select(_srgb_s0_y > -1, _srgb_s0_y, -1) * 768))) + 768)]
                _t271 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766) > -1, hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766), -1) + (hcl.select(_srgb_s0_y > -1, _srgb_s0_y, -1) * 768))) + 769)]
                _t273 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765) > -2, hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765), -2) + (hcl.select(_srgb_s0_y > -2, _srgb_s0_y, -2) * 768))) + 1538)]
                _t274 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767) > 0, hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767), 0) + (hcl.select(_srgb_s0_y > -2, _srgb_s0_y, -2) * 768))) + 1536)]
                _t275 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766) > -1, hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766), -1) + (hcl.select(_srgb_s0_y > -2, _srgb_s0_y, -2) * 768))) + 1537)]
                _t276 = (((hcl.select((float(0.040450) < _t263), hcl.power(((_t263 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t263 * float(0.077399))) + (hcl.select((float(0.040450) < _t265), hcl.power(((_t265 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t265 * float(0.077399))) + hcl.select((float(0.040450) < _t267), hcl.power(((_t267 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t267 * float(0.077399))))) + (hcl.select((float(0.040450) < _t269), hcl.power(((_t269 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t269 * float(0.077399))) + (hcl.select((float(0.040450) < _t270), hcl.power(((_t270 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t270 * float(0.077399))) + hcl.select((float(0.040450) < _t271), hcl.power(((_t271 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t271 * float(0.077399)))))) + (hcl.select((float(0.040450) < _t273), hcl.power(((_t273 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t273 * float(0.077399))) + (hcl.select((float(0.040450) < _t274), hcl.power(((_t274 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t274 * float(0.077399))) + hcl.select((float(0.040450) < _t275), hcl.power(((_t275 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t275 * float(0.077399))))))
                out[0] = hcl.select((float(0.028177) < _t276), ((hcl.power((_t276 * float(0.111111)), float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t276 * float(1.435556)))
                # with hcl.if_ (x >= 4):
                #     out[0] = 3
        with hcl.elif_ (_srgb_s0_y < 0 + 1278): # y for
            with hcl.if_ (_srgb_s0_x < 0 + 0): # x for
                _t277 = ((_srgb_s0_c * 983040) + (hcl.select(_srgb_s0_x > -2, _srgb_s0_x, -2) + (_srgb_s0_y * 768)))
                _t278 = _input[(_t277 + 2)]
                _t279 = ((_srgb_s0_c * 983040) + (hcl.select(_srgb_s0_x > 0, _srgb_s0_x, 0) + (_srgb_s0_y * 768)))
                _t280 = _input[_t279]
                _t281 = ((_srgb_s0_c * 983040) + (hcl.select(_srgb_s0_x > -1, _srgb_s0_x, -1) + (_srgb_s0_y * 768)))
                _t282 = _input[(_t281 + 1)]
                _t283 = _input[(_t277 + 770)]
                _t284 = _input[(_t279 + 768)]
                _t285 = _input[(_t281 + 769)]
                _t286 = _input[(_t277 + 1538)]
                _t287 = _input[(_t279 + 1536)]
                _t288 = _input[(_t281 + 1537)]
                _t289 = (((hcl.select((float(0.040450) < _t278), hcl.power(((_t278 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t278 * float(0.077399))) + (hcl.select((float(0.040450) < _t280), hcl.power(((_t280 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t280 * float(0.077399))) + hcl.select((float(0.040450) < _t282), hcl.power(((_t282 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t282 * float(0.077399))))) + (hcl.select((float(0.040450) < _t283), hcl.power(((_t283 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t283 * float(0.077399))) + (hcl.select((float(0.040450) < _t284), hcl.power(((_t284 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t284 * float(0.077399))) + hcl.select((float(0.040450) < _t285), hcl.power(((_t285 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t285 * float(0.077399)))))) + (hcl.select((float(0.040450) < _t286), hcl.power(((_t286 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t286 * float(0.077399))) + (hcl.select((float(0.040450) < _t287), hcl.power(((_t287 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t287 * float(0.077399))) + hcl.select((float(0.040450) < _t288), hcl.power(((_t288 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t288 * float(0.077399))))))
                out[0] = hcl.select((float(0.028177) < _t289), ((hcl.power((_t289 * float(0.111111)), float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t289 * float(1.435556)))
            with hcl.elif_ (_srgb_s0_x < 0 + 766): # x for
                # t11 = 2
                # t12 = hcl.select(5 > t11, 3, 4) #bug?
                
                

                _t290 = ((_srgb_s0_c * 983040) + ((_srgb_s0_y * 768) + _srgb_s0_x))
                _t291 = _input[(_t290 + 2)]
                _t292 = _input[_t290]
                _t293 = _input[(_t290 + 1)]
                _t294 = _input[(_t290 + 770)]
                _t295 = _input[(_t290 + 768)]
                _t296 = _input[(_t290 + 769)]
                _t297 = _input[(_t290 + 1538)]
                _t298 = _input[(_t290 + 1536)]
                _t299 = _input[(_t290 + 1537)]
                _t300 = (((hcl.select((float(0.040450) < _t291), hcl.power(((_t291 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t291 * float(0.077399))) + (hcl.select((float(0.040450) < _t292), hcl.power(((_t292 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t292 * float(0.077399))) + hcl.select((float(0.040450) < _t293), hcl.power(((_t293 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t293 * float(0.077399))))) + (hcl.select((float(0.040450) < _t294), hcl.power(((_t294 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t294 * float(0.077399))) + (hcl.select((float(0.040450) < _t295), hcl.power(((_t295 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t295 * float(0.077399))) + hcl.select((float(0.040450) < _t296), hcl.power(((_t296 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t296 * float(0.077399)))))) + (hcl.select((float(0.040450) < _t297), hcl.power(((_t297 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t297 * float(0.077399))) + (hcl.select((float(0.040450) < _t298), hcl.power(((_t298 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t298 * float(0.077399))) + hcl.select((float(0.040450) < _t299), hcl.power(((_t299 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t299 * float(0.077399))))))
                out[0] = hcl.select((float(0.028177) < _t300), ((hcl.power((_t300 * float(0.111111)), float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t300 * float(1.435556)))

            with hcl.elif_ (_srgb_s0_x < 766+2):# x for
                _t301 = ((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765) > -2, hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765), -2) + (_srgb_s0_y * 768)))
                _t302 = _input[(_t301 + 2)]
                _t303 = ((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767) > 0, hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767), 0) + (_srgb_s0_y * 768)))
                _t304 = _input[_t303]
                _t305 = ((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766) > -1, hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766), -1) + (_srgb_s0_y * 768)))
                _t306 = _input[(_t305 + 1)]
                _t307 = _input[(_t301 + 770)]
                _t308 = _input[(_t303 + 768)]
                _t309 = _input[(_t305 + 769)]
                _t310 = _input[(_t301 + 1538)]
                _t311 = _input[(_t303 + 1536)]
                _t312 = _input[(_t305 + 1537)]
                _t313 = (((hcl.select((float(0.040450) < _t302), hcl.power(((_t302 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t302 * float(0.077399))) + (hcl.select((float(0.040450) < _t304), hcl.power(((_t304 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t304 * float(0.077399))) + hcl.select((float(0.040450) < _t306), hcl.power(((_t306 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t306 * float(0.077399))))) + (hcl.select((float(0.040450) < _t307), hcl.power(((_t307 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t307 * float(0.077399))) + (hcl.select((float(0.040450) < _t308), hcl.power(((_t308 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t308 * float(0.077399))) + hcl.select((float(0.040450) < _t309), hcl.power(((_t309 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t309 * float(0.077399)))))) + (hcl.select((float(0.040450) < _t310), hcl.power(((_t310 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t310 * float(0.077399))) + (hcl.select((float(0.040450) < _t311), hcl.power(((_t311 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t311 * float(0.077399))) + hcl.select((float(0.040450) < _t312), hcl.power(((_t312 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t312 * float(0.077399))))))
                out[0] = hcl.select((float(0.028177) < _t313), ((hcl.power((_t313 * float(0.111111)), float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t313 * float(1.435556)))
        with hcl.elif_ (_srgb_s0_y < 1278 + 2):
            _t316 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765) > -2, hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765), -2) + (hcl.select(hcl.select(_srgb_s0_y < 1279, _srgb_s0_y, 1279) > 0, hcl.select(_srgb_s0_y < 1279, _srgb_s0_y, 1279), 0) * 768))) + 2)]
            _t318 = _input[((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767) > 0, hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767), 0) + (hcl.select(hcl.select(_srgb_s0_y < 1279, _srgb_s0_y, 1279) > 0, hcl.select(_srgb_s0_y < 1279, _srgb_s0_y, 1279), 0) * 768)))]
            _t320 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766) > -1, hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766), -1) + (hcl.select(hcl.select(_srgb_s0_y < 1279, _srgb_s0_y, 1279) > 0, hcl.select(_srgb_s0_y < 1279, _srgb_s0_y, 1279), 0) * 768))) + 1)]
            _t322 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765) > -2, hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765), -2) + (hcl.select(hcl.select(_srgb_s0_y < 1278, _srgb_s0_y, 1278) > -1, hcl.select(_srgb_s0_y < 1278, _srgb_s0_y, 1278), -1) * 768))) + 770)]
            _t323 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767) > 0, hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767), 0) + (hcl.select(hcl.select(_srgb_s0_y < 1278, _srgb_s0_y, 1278) > -1, hcl.select(_srgb_s0_y < 1278, _srgb_s0_y, 1278), -1) * 768))) + 768)]
            _t324 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766) > -1, hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766), -1) + (hcl.select(hcl.select(_srgb_s0_y < 1278, _srgb_s0_y, 1278) > -1, hcl.select(_srgb_s0_y < 1278, _srgb_s0_y, 1278), -1) * 768))) + 769)]
            _t326 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765) > -2, hcl.select(_srgb_s0_x < 765, _srgb_s0_x, 765), -2) + (hcl.select(hcl.select(_srgb_s0_y < 1277, _srgb_s0_y, 1277) > -2, hcl.select(_srgb_s0_y < 1277, _srgb_s0_y, 1277), -2) * 768))) + 1538)]
            _t327 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767) > 0, hcl.select(_srgb_s0_x < 767, _srgb_s0_x, 767), 0) + (hcl.select(hcl.select(_srgb_s0_y < 1277, _srgb_s0_y, 1277) > -2, hcl.select(_srgb_s0_y < 1277, _srgb_s0_y, 1277), -2) * 768))) + 1536)]
            _t328 = _input[(((_srgb_s0_c * 983040) + (hcl.select(hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766) > -1, hcl.select(_srgb_s0_x < 766, _srgb_s0_x, 766), -1) + (hcl.select(hcl.select(_srgb_s0_y < 1277, _srgb_s0_y, 1277) > -2, hcl.select(_srgb_s0_y < 1277, _srgb_s0_y, 1277), -2) * 768))) + 1537)]
            _t329 = (((hcl.select((float(0.040450) < _t316), hcl.power(((_t316 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t316 * float(0.077399))) + (hcl.select((float(0.040450) < _t318), hcl.power(((_t318 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t318 * float(0.077399))) + hcl.select((float(0.040450) < _t320), hcl.power(((_t320 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t320 * float(0.077399))))) + (hcl.select((float(0.040450) < _t322), hcl.power(((_t322 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t322 * float(0.077399))) + (hcl.select((float(0.040450) < _t323), hcl.power(((_t323 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t323 * float(0.077399))) + hcl.select((float(0.040450) < _t324), hcl.power(((_t324 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t324 * float(0.077399)))))) + (hcl.select((float(0.040450) < _t326), hcl.power(((_t326 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t326 * float(0.077399))) + (hcl.select((float(0.040450) < _t327), hcl.power(((_t327 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t327 * float(0.077399))) + hcl.select((float(0.040450) < _t328), hcl.power(((_t328 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t328 * float(0.077399))))))
            out[0] = hcl.select((float(0.028177) < _t329), ((hcl.power((_t329 * float(0.111111)), float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t329 * float(1.435556)))

        return out[0]
        # out[0] = (let t2217 = input[(((srgb.s0._0*983040) + (max(min(srgb.s0.x, 765), -2) + (max(srgb.s0.y, 0)*768))) + 2)] in (let t2219 = input[((srgb.s0._0*983040) + (max(min(srgb.s0.x, 767), 0) + (max(srgb.s0.y, 0)*768)))] in (let t2221 = input[(((srgb.s0._0*983040) + (max(min(srgb.s0.x, 766), -1) + (max(srgb.s0.y, 0)*768))) + 1)] in (let t2223 = input[(((srgb.s0._0*983040) + (max(min(srgb.s0.x, 765), -2) + (max(srgb.s0.y, -1)*768))) + 770)] in (let t2224 = input[(((srgb.s0._0*983040) + (max(min(srgb.s0.x, 767), 0) + (max(srgb.s0.y, -1)*768))) + 768)] in (let t2225 = input[(((srgb.s0._0*983040) + (max(min(srgb.s0.x, 766), -1) + (max(srgb.s0.y, -1)*768))) + 769)] in (let t2227 = input[(((srgb.s0._0*983040) + (max(min(srgb.s0.x, 765), -2) + (max(srgb.s0.y, -2)*768))) + 1538)] in (let t2228 = input[(((srgb.s0._0*983040) + (max(min(srgb.s0.x, 767), 0) + (max(srgb.s0.y, -2)*768))) + 1536)] in (let t2229 = input[(((srgb.s0._0*983040) + (max(min(srgb.s0.x, 766), -1) + (max(srgb.s0.y, -2)*768))) + 1537)] in (let t2230 = (((select((0.040450f < t2217), pow_f32(((t2217*0.947867f) + 0.052133f), 2.400000f), (t2217*0.077399f)) + (select((0.040450f < t2219), pow_f32(((t2219*0.947867f) + 0.052133f), 2.400000f), (t2219*0.077399f)) + select((0.040450f < t2221), pow_f32(((t2221*0.947867f) + 0.052133f), 2.400000f), (t2221*0.077399f)))) + (select((0.040450f < t2223), pow_f32(((t2223*0.947867f) + 0.052133f), 2.400000f), (t2223*0.077399f)) + (select((0.040450f < t2224), pow_f32(((t2224*0.947867f) + 0.052133f), 2.400000f), (t2224*0.077399f)) + select((0.040450f < t2225), pow_f32(((t2225*0.947867f) + 0.052133f), 2.400000f), (t2225*0.077399f))))) + (select((0.040450f < t2227), pow_f32(((t2227*0.947867f) + 0.052133f), 2.400000f), (t2227*0.077399f)) + (select((0.040450f < t2228), pow_f32(((t2228*0.947867f) + 0.052133f), 2.400000f), (t2228*0.077399f)) + select((0.040450f < t2229), pow_f32(((t2229*0.947867f) + 0.052133f), 2.400000f), (t2229*0.077399f))))) in select((0.028177f < t2230), ((pow_f32((t2230*0.111111f), 0.416667f)*1.055000f) + -0.055000f), (t2230*1.435556f))))))))))))
    linear_blur = hcl.compute((768, 1280, 3, ), lambda x,y,c: final(x, y, c, input), name = "linear_blur", dtype = hcl.Float())  
    return linear_blur

s = hcl.create_schedule([input], linear_blur)
f = hcl.build(s)
print(hcl.lower(s))

import numpy as np 
np_in = np.transpose(np.load("input.npy"), (2,1,0)).flatten()
hcl_in = hcl.asarray(np_in, dtype = hcl.Float())
hcl_out = hcl.asarray(np.zeros((768, 1280, 3)).astype('float32'), dtype = hcl.Float())
# need to specify the data type of the HeteroCL array instead of numpy array, otherwise there will be data type error

f(hcl_in, hcl_out)
print('execute done')

np_out = hcl_out.asnumpy()
np.save('output_heterocl_target.npy', np_out)



# def linear_blur(input):
#     def sRGBtoLinear(srgb):
#         linear = hcl.local(0, dtype = hcl.Float())
#         with hcl.if_(srgb <= 0.04045):
#             linear[0] = srgb / 12.92
#         with hcl.else_():
#             # linear[0] = ((srgb + 0.055) / 1.055 ) ** 2.4
#             linear[0] = hcl.power((srgb + 0.055) / 1.055 , 2.4)
#         return linear[0]
#     linear_tmp = hcl.compute(input.shape, lambda x,y,c: sRGBtoLinear(input[x,y,c]), "sRGBtoLinear", dtype = hcl.Float())

#     # Halide:BoundaryConditions::repeat_edge   means the edge have the same value with the pixel next to it? 
#     # e.g. blur_tmp[1280,768,0] = (blur_tmp[1280,768,0]+blur_tmp[1281,768,0]+blur_tmp[1282,768,0]) / 3 = blur_tmp[1280,768,0]?
#     # didn't find the related source code in Halide
#     def blur_x(linear_tmp, x, y, c):
#         blurx = hcl.local(0, dtype = hcl.Float())
#         with hcl.if_(x < input.shape[0] - 2): # x != 766 or 767 
#             blurx[0] = (linear_tmp[x,y,c] + linear_tmp[x+1,y,c] + linear_tmp[x+2,y,c]) / 3
#         with hcl.elif_(x == input.shape[0] - 2): # x = 766 
#             blurx[0] = (linear_tmp[x,y,c] + linear_tmp[x+1,y,c] + linear_tmp[x+1,y,c]) / 3
#         with hcl.else_(): # x = 767
#             blurx[0] = linear_tmp[x,y,c]
#         return blurx[0]
#     blur_x = hcl.compute(input.shape, lambda x,y,c: blur_x(linear_tmp, x, y, c), "blur_x", dtype = hcl.Float())

#     def blur_y(blur_x, linear_tmp, x, y, c):
#         blury = hcl.local(0, dtype = hcl.Float())
#         with hcl.if_(y < input.shape[1] - 2): # y != 1278 or 1279
#             blury[0] = (blur_x[x,y,c] + blur_x[x,y+1,c] + blur_x[x,y+2,c]) / 3
#         with hcl.elif_(y == input.shape[1] - 2): # y = 1278
#             blury[0] = (blur_x[x,y,c] + blur_x[x,y+1,c] + linear_tmp[x,y+1,c]) / 3
#         with hcl.else_(): # y = 1279
#             blury[0] = (blur_x[x,y,c] + linear_tmp[x,y,c] + linear_tmp[x,y,c]) / 3
#         return blury[0]
#     blur_y = hcl.compute(input.shape, lambda x,y,c: blur_y(blur_x, linear_tmp, x, y, c), "blur_y", dtype = hcl.Float())
#     # actualy, blur_x + blur_y is a blur 3*3.

#     def LineartosRGB(linear):
#         srgb = hcl.local(0, dtype = hcl.Float())
#         with hcl.if_(linear <= 0.0031308):
#             srgb[0] = linear * 12.92
#         with hcl.else_():
#             # srgb[0] = 1.055 * (linear ** (1/2.4)) - 0.055
#             srgb[0] = 1.055 * hcl.power(linear, (1/2.4)) - 0.055
#         return srgb[0]  
#     output = hcl.compute(input.shape, lambda x,y,c: LineartosRGB(blur_y[x,y,c]), "LineartosRGB", dtype = hcl.Float())  
#     # return output
#     # output = hcl.compute(input.shape, lambda x,y,c: input[x,y,c], "LineartosRGB", dtype = hcl.Float())
#     return output

# s = hcl.create_schedule([input], linear_blur)
# print(hcl.lower(s))

# # f = hcl.build(s, target='vhls')
# f = hcl.build(s)
# print(f)

# print('done')

# np_in = np.load("input.npy")
# hcl_in = hcl.asarray(np_in, dtype = hcl.Float())
# hcl_out = hcl.asarray(np.zeros(input.shape).astype('float32'), dtype = hcl.Float())
# # need to specify the data type of the HeteroCL array instead of numpy array, otherwise there will be data type error

# f(hcl_in, hcl_out)
# print('execute done')

# np_out = hcl_out.asnumpy()
# np.save('output_heterocl.npy', np_out)