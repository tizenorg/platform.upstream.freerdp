%bcond_with x

Name:           freerdp
Version:        1.1.0
Release:        0
Summary:        Remote Desktop Protocol functionality
License:        Apache-2.0
Group:          Graphics & UI Framework/Development
Url:            http://www.freerdp.com/

Source0:         %name-%version.tar.xz
Source1: 	freerdp.manifest
BuildRequires:	cmake
BuildRequires:  libtool >= 2.2
BuildRequires:  zlib-devel
BuildRequires:  pkgconfig
BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(alsa)
%if %{with x}
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(xcursor)
BuildRequires:  pkgconfig(xdamage)
BuildRequires:  pkgconfig(xext)
BuildRequires:  pkgconfig(xi)
BuildRequires:  pkgconfig(xinerama)
BuildRequires:  pkgconfig(xkbfile)
BuildRequires:  pkgconfig(xrandr)
BuildRequires:  pkgconfig(xrender)
BuildRequires:  pkgconfig(xtst)
BuildRequires:  pkgconfig(xv)
%endif

%description
FreeRDP is a free implementation of the Remote Desktop Protocol (RDP)
according to the Microsoft Open Specifications.

%package devel
Summary: Development files for package %{name}
Group:   Graphics & UI Framework/Development
Requires: freerdp = %{version}-%{release}
%description devel
This package provides header files and other developer related files
for package %{name}.

%prep
%setup -q
cp %{SOURCE1} .

%build
cmake \
        -DCMAKE_INSTALL_PREFIX:PATH=/usr \
%if %{with x}
        -DWITH_X11:BOOL=ON \
        -DWITH_CLIENT:BOOL=ON \
%else
        -DWITH_X11:BOOL=OFF \
        -DWITH_CLIENT:BOOL=OFF \
%endif
        -DWITH_SERVER:BOOL=ON \
        -DWITH_ALSA:BOOL=ON \
        -DWITH_SSE2:BOOL=OFF \
        .
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT INSTALL='install -p'
install -p -D winpr/tools/makecert/cli/winpr-makecert $RPM_BUILD_ROOT%{_bindir}/winpr-makecert
rm $RPM_BUILD_ROOT%{_libdir}/libwinpr-makecert-tool.a
install -m 644 -p -D winpr/include/winpr/config.h $RPM_BUILD_ROOT%{_includedir}/winpr/config.h


%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%defattr(-,root,root)
%license LICENSE
%if %{with x}
%{_bindir}/xfreerdp
%{_bindir}/xfreerdp-server
%endif
%{_bindir}/winpr-makecert
%{_libdir}/lib*.so.*

%files devel
%manifest %{name}.manifest
%{_includedir}/freerdp
%{_includedir}/winpr
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/freerdp*.pc


%changelog
