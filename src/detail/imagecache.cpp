/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "egt/detail/imagecache.h"
#include "egt/detail/resource.h"
#include "egt/utils.h"
#include <cassert>
#ifdef HAVE_LIBMAGIC
#include <magic.h>
#endif
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <unistd.h>

#include "cairo_bmp.h"

#ifdef HAVE_LIBJPEG
#ifdef __cplusplus
extern "C" {
#endif
#include "cairo_jpg.h"
#ifdef __cplusplus
}
#endif
#endif

using namespace std;

namespace egt
{
inline namespace v1
{
namespace detail
{

static vector<string> image_paths;

void add_search_path(const std::string& path)
{
    if (path.empty())
        return;

    auto newpath = path;

    if (*newpath.rbegin() != '/')
        newpath += '/';

    if (find(image_paths.begin(), image_paths.end(), newpath) == image_paths.end())
        image_paths.push_back(newpath);
}

std::string resolve_file_path(const std::string& filename)
{
    if (filename.empty())
        return filename;

    // we don't resolve absolute paths
    if (filename[0] == '/')
        return filename;

    for (auto& path : image_paths)
    {
        auto test = path + filename;
        struct stat buf;
        if (!stat(test.c_str(), &buf))
        {
            DBG("found file " << test);
            return test;
        }
    }

    return filename;
}

shared_cairo_surface_t ImageCache::get(const std::string& filename,
                                       float hscale, float vscale, bool approximate)
{
    if (approximate)
    {
        hscale = ImageCache::round(hscale, 0.01);
        vscale = ImageCache::round(vscale, 0.01);
    }

    const string name = id(filename, hscale, vscale);

    auto i = m_cache.find(name);
    if (i != m_cache.end())
        return i->second;

    DBG("image cache miss: " << filename << " hscale:" << hscale << " vscale:" << vscale);

    shared_cairo_surface_t image;

    if (hscale == 1.0 && vscale == 1.0)
    {
        assert(CAIRO_HAS_PNG_FUNCTIONS == 1);

        if (filename.compare(0, 1, ":") == 0)
        {
            string name = filename;
            name.erase(0, 1);

            auto mimetype = get_mime_type(read_resource_data(name.c_str()),
                                          read_resource_length(name.c_str()));
            DBG("mimetype of " << name << " is " << mimetype);

            if (mimetype == "image/png")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_png_stream(
                                read_resource_stream, (void*)name.c_str()),
                            cairo_surface_destroy);
            }
#ifdef HAVE_LIBJPEG
            else if (mimetype == "image/jpeg")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_jpeg_stream(
                                read_resource_stream, (void*)name.c_str()),
                            cairo_surface_destroy);
            }
#endif
            else if (mimetype == "image/x-ms-bmp")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_bmp_stream(
                                read_resource_stream, (void*)name.c_str()),
                            cairo_surface_destroy);
            }
            else
            {
                ERR("could not load resource " << name);
                throw std::runtime_error("unsupported file type");
            }
        }
        else if (filename.compare(0, 1, "@") == 0)
        {
            string name = filename;
            name.erase(0, 1);
            name = resolve_file_path(name);

            auto mimetype = get_mime_type(name);
            DBG("mimetype of " << name << " is " << mimetype);

            if (mimetype == "image/png")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_png(name.c_str()),
                            cairo_surface_destroy);
            }
#ifdef HAVE_LIBJPEG
            else if (mimetype == "image/jpeg")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_jpeg(name.c_str()),
                            cairo_surface_destroy);
            }
#endif
            else if (mimetype == "image/x-ms-bmp")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_bmp(name.c_str()),
                            cairo_surface_destroy);
            }
            else
            {
                ERR("could not load file " << name);
                throw std::runtime_error("unsupported file type");
            }
        }
        else
        {
            string name = resolve_file_path(filename);
            DBG("loading: " << name);

            auto mimetype = get_mime_type(name);
            DBG("mimetype of " << name << " is " << mimetype);

            if (mimetype == "image/png")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_png(name.c_str()),
                            cairo_surface_destroy);
            }
#ifdef HAVE_LIBJPEG
            else if (mimetype == "image/jpeg")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_jpeg(name.c_str()),
                            cairo_surface_destroy);
            }
#endif
            else if (mimetype == "image/x-ms-bmp")
            {
                image = shared_cairo_surface_t(
                            cairo_image_surface_create_from_bmp(name.c_str()),
                            cairo_surface_destroy);
            }
            else
            {
                ERR("could not load file " << name);
                throw std::runtime_error("unsupported file type");
            }
        }
    }
    else
    {
        shared_cairo_surface_t back = get(filename, 1.0);

        double width = cairo_image_surface_get_width(back.get());
        double height = cairo_image_surface_get_height(back.get());

        image = scale_surface(back,
                              width, height,
                              width * hscale,
                              height * vscale);
    }

    if (cairo_surface_status(image.get()) != CAIRO_STATUS_SUCCESS)
    {
        stringstream ss;
        ss << cairo_status_to_string(cairo_surface_status(image.get()))
           << ": " << filename;
        throw std::runtime_error(ss.str());
    }

    m_cache.insert(std::make_pair(name, image));

    return image;
}

void ImageCache::clear()
{
    m_cache.clear();
}

float ImageCache::round(float v, float fraction)
{
    return floor(v) + floor((v - floor(v)) / fraction) * fraction;
}

string ImageCache::id(const string& filename, float hscale, float vscale)
{
    ostringstream ss;
    ss << filename << "-" << hscale * 100. << "-" << vscale * 100.;

    return ss.str();
}

shared_cairo_surface_t
ImageCache::scale_surface(shared_cairo_surface_t old_surface,
                          int old_width, int old_height,
                          int new_width, int new_height)
{
    auto new_surface = shared_cairo_surface_t(
                           cairo_surface_create_similar(old_surface.get(),
                                   CAIRO_CONTENT_COLOR_ALPHA,
                                   new_width,
                                   new_height),
                           cairo_surface_destroy);
    auto cr = shared_cairo_t(cairo_create(new_surface.get()),
                             cairo_destroy);

    /* Scale *before* setting the source surface (1) */
    cairo_scale(cr.get(),
                (double)new_width / old_width,
                (double)new_height / old_height);
    cairo_set_source_surface(cr.get(), old_surface.get(), 0, 0);

    /* To avoid getting the edge pixels blended with 0 alpha, which would
     * occur with the default EXTEND_NONE. Use EXTEND_PAD for 1.2 or newer (2)
     */
    cairo_pattern_set_extend(cairo_get_source(cr.get()), CAIRO_EXTEND_REFLECT);

    /* Replace the destination with the source instead of overlaying */
    cairo_set_operator(cr.get(), CAIRO_OPERATOR_SOURCE);

    /* Do the actual drawing */
    cairo_paint(cr.get());

    return new_surface;
}

std::string ImageCache::get_mime_type(const void* buffer, size_t length)
{
    string result;
#ifdef HAVE_LIBMAGIC
    magic_t magic = magic_open(MAGIC_MIME_TYPE);

    if (magic)
    {
        if (!magic_load(magic, NULL))
        {
            auto mime = magic_buffer(magic, buffer, length);
            if (mime)
                result = mime;
        }

        magic_close(magic);
    }
#endif
    return result;
}

std::string ImageCache::get_mime_type(const std::string& filename)
{
    string result;
#ifdef HAVE_LIBMAGIC
    magic_t magic = magic_open(MAGIC_MIME_TYPE);

    if (magic)
    {
        if (!magic_load(magic, NULL))
        {
            auto mime = magic_file(magic, filename.c_str());
            if (mime)
                result = mime;
        }

        magic_close(magic);
    }
#else
    if (filename.find("png") != std::string::npos)
        result = "image/png";
    else if (filename.find("jpg") != std::string::npos)
        result = "image/jpeg";
#endif
    return result;
}

ImageCache& image_cache()
{
    static ImageCache cache;
    return cache;
}
}
}
}