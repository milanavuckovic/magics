#ifndef BOOST_GEOMETRY_PROJECTIONS_CASS_HPP
#define BOOST_GEOMETRY_PROJECTIONS_CASS_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2011 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels (Geodan, Amsterdam)

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_mlfn.hpp>

#include <boost/geometry/extensions/gis/projections/epsg_traits.hpp>

namespace boost { namespace geometry { namespace projection
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace cass{
            static const double EPS10 = 1e-10;
            static const double C1 = .16666666666666666666;
            static const double C2 = .00833333333333333333;
            static const double C3 = .04166666666666666666;
            static const double C4 = .33333333333333333333;
            static const double C5 = .06666666666666666666;

            struct par_cass
            {
                double m0;
                double n;
                double t;
                double a1;
                double c;
                double r;
                double dd;
                double d2;
                double a2;
                double tn;
                double en[EN_SIZE];
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_cass_ellipsoid : public base_t_fi<base_cass_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                mutable par_cass m_proj_parm;

                inline base_cass_ellipsoid(const Parameters& par)
                    : base_t_fi<base_cass_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    xy_y = pj_mlfn(lp_lat, this->m_proj_parm.n = sin(lp_lat), this->m_proj_parm.c = cos(lp_lat), this->m_proj_parm.en);
                    this->m_proj_parm.n = 1./sqrt(1. - this->m_par.es * this->m_proj_parm.n * this->m_proj_parm.n);
                    this->m_proj_parm.tn = tan(lp_lat); this->m_proj_parm.t = this->m_proj_parm.tn * this->m_proj_parm.tn;
                    this->m_proj_parm.a1 = lp_lon * this->m_proj_parm.c;
                    this->m_proj_parm.c *= this->m_par.es * this->m_proj_parm.c / (1 - this->m_par.es);
                    this->m_proj_parm.a2 = this->m_proj_parm.a1 * this->m_proj_parm.a1;
                    xy_x = this->m_proj_parm.n * this->m_proj_parm.a1 * (1. - this->m_proj_parm.a2 * this->m_proj_parm.t *
                        (C1 - (8. - this->m_proj_parm.t + 8. * this->m_proj_parm.c) * this->m_proj_parm.a2 * C2));
                    xy_y -= this->m_proj_parm.m0 - this->m_proj_parm.n * this->m_proj_parm.tn * this->m_proj_parm.a2 *
                        (.5 + (5. - this->m_proj_parm.t + 6. * this->m_proj_parm.c) * this->m_proj_parm.a2 * C3);
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double ph1;

                    ph1 = pj_inv_mlfn(this->m_proj_parm.m0 + xy_y, this->m_par.es, this->m_proj_parm.en);
                    this->m_proj_parm.tn = tan(ph1); this->m_proj_parm.t = this->m_proj_parm.tn * this->m_proj_parm.tn;
                    this->m_proj_parm.n = sin(ph1);
                    this->m_proj_parm.r = 1. / (1. - this->m_par.es * this->m_proj_parm.n * this->m_proj_parm.n);
                    this->m_proj_parm.n = sqrt(this->m_proj_parm.r);
                    this->m_proj_parm.r *= (1. - this->m_par.es) * this->m_proj_parm.n;
                    this->m_proj_parm.dd = xy_x / this->m_proj_parm.n;
                    this->m_proj_parm.d2 = this->m_proj_parm.dd * this->m_proj_parm.dd;
                    lp_lat = ph1 - (this->m_proj_parm.n * this->m_proj_parm.tn / this->m_proj_parm.r) * this->m_proj_parm.d2 *
                        (.5 - (1. + 3. * this->m_proj_parm.t) * this->m_proj_parm.d2 * C3);
                    lp_lon = this->m_proj_parm.dd * (1. + this->m_proj_parm.t * this->m_proj_parm.d2 *
                        (-C4 + (1. + 3. * this->m_proj_parm.t) * this->m_proj_parm.d2 * C5)) / cos(ph1);
                }
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_cass_spheroid : public base_t_fi<base_cass_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                mutable par_cass m_proj_parm;

                inline base_cass_spheroid(const Parameters& par)
                    : base_t_fi<base_cass_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    xy_x = asin(cos(lp_lat) * sin(lp_lon));
                    xy_y = atan2(tan(lp_lat) , cos(lp_lon)) - this->m_par.phi0;
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    lp_lat = asin(sin(this->m_proj_parm.dd = xy_y + this->m_par.phi0) * cos(xy_x));
                    lp_lon = atan2(tan(xy_x), cos(this->m_proj_parm.dd));
                }
            };

            // Cassini
            template <typename Parameters>
            void setup_cass(Parameters& par, par_cass& proj_parm)
            {
                if (par.es) {
                    pj_enfn(par.es, proj_parm.en);
                    proj_parm.m0 = pj_mlfn(par.phi0, sin(par.phi0), cos(par.phi0), proj_parm.en);
                // par.inv = e_inverse;
                // par.fwd = e_forward;
                } else {
                // par.inv = s_inverse;
                // par.fwd = s_forward;
                }
            }

        }} // namespace detail::cass
    #endif // doxygen

    /*!
        \brief Cassini projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
        \par Example
        \image html ex_cass.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct cass_ellipsoid : public detail::cass::base_cass_ellipsoid<Geographic, Cartesian, Parameters>
    {
        inline cass_ellipsoid(const Parameters& par) : detail::cass::base_cass_ellipsoid<Geographic, Cartesian, Parameters>(par)
        {
            detail::cass::setup_cass(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Cassini projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
        \par Example
        \image html ex_cass.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct cass_spheroid : public detail::cass::base_cass_spheroid<Geographic, Cartesian, Parameters>
    {
        inline cass_spheroid(const Parameters& par) : detail::cass::base_cass_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::cass::setup_cass(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class cass_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    if (par.es)
                        return new base_v_fi<cass_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                    else
                        return new base_v_fi<cass_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void cass_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("cass", new cass_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    // Create EPSG specializations
    // (Proof of Concept, only for some)

    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<2066, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=11.25217861111111 +lon_0=-60.68600888888889 +x_0=37718.66159325 +y_0=36209.91512952 +a=6378293.645208759 +b=6356617.987679838 +to_meter=0.201166195164";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<2099, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=25.38236111111111 +lon_0=50.76138888888889 +x_0=100000 +y_0=100000 +ellps=helmert +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<2314, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=10.44166666666667 +lon_0=-61.33333333333334 +x_0=86501.46392052001 +y_0=65379.0134283 +a=6378293.645208759 +b=6356617.987679838 +to_meter=0.3047972654";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3068, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=52.41864827777778 +lon_0=13.62720366666667 +x_0=40000 +y_0=10000 +ellps=bessel +datum=potsdam +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3140, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=-18 +lon_0=178 +x_0=109435.392 +y_0=141622.272 +a=6378306.3696 +b=6356571.996 +towgs84=51,391,-36,0,0,0,0 +to_meter=0.201168";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3366, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=22.31213333333334 +lon_0=114.1785555555556 +x_0=40243.57775604237 +y_0=19069.93351512578 +a=6378293.645208759 +b=6356617.987679838 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3377, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=2.121679744444445 +lon_0=103.4279362361111 +x_0=-14810.562 +y_0=8758.32 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3378, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=2.682347636111111 +lon_0=101.9749050416667 +x_0=3673.785 +y_0=-4240.573 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3379, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=3.769388088888889 +lon_0=102.3682989833333 +x_0=-7368.228 +y_0=6485.858 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3380, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=3.68464905 +lon_0=101.3891079138889 +x_0=-34836.161 +y_0=56464.049 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3381, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=4.9762852 +lon_0=103.070275625 +x_0=19594.245 +y_0=3371.895 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3382, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=5.421517541666667 +lon_0=100.3443769638889 +x_0=-23.414 +y_0=62.283 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3383, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=5.964672713888889 +lon_0=100.6363711111111 +x_0=0 +y_0=0 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3384, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=4.859063022222222 +lon_0=100.8154105861111 +x_0=-1.769 +y_0=133454.779 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3385, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=5.972543658333334 +lon_0=102.2952416694444 +x_0=13227.851 +y_0=8739.894 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3407, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=22.31213333333334 +lon_0=114.1785555555556 +x_0=40243.57775604237 +y_0=19069.93351512578 +a=6378293.645208759 +b=6356617.987679838 +to_meter=0.3047972654";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<24500, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=1.287646666666667 +lon_0=103.8530022222222 +x_0=30000 +y_0=30000 +a=6377304.063 +b=6356103.038993155 +towgs84=-11,851,5,0,0,0,0 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<28191, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=31.73409694444445 +lon_0=35.21208055555556 +x_0=170251.555 +y_0=126867.909 +a=6378300.789 +b=6356566.435 +towgs84=-275.722,94.7824,340.894,-8.001,-4.42,-11.821,1 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<28193, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=31.73409694444445 +lon_0=35.21208055555556 +x_0=170251.555 +y_0=1126867.909 +a=6378300.789 +b=6356566.435 +towgs84=-275.722,94.7824,340.894,-8.001,-4.42,-11.821,1 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<30200, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=10.44166666666667 +lon_0=-61.33333333333334 +x_0=86501.46392051999 +y_0=65379.0134283 +a=6378293.645208759 +b=6356617.987679838 +to_meter=0.201166195164";
        }
    };


    #endif // doxygen

}}} // namespace boost::geometry::projection

#endif // BOOST_GEOMETRY_PROJECTIONS_CASS_HPP

