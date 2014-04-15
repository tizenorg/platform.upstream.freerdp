%bcond_with x

Name:           freerdp
Version:        1.1.0+beta+2013071101
Release:        0
Summary:        Remote Desktop Protocol client
Group:          Group: Applications/Other

License:        Apache-2.0
URL:            http://www.freerdp.com/
Source0:        http://pub.freerdp.com/releases/%{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  cups-devel
BuildRequires:  desktop-file-utils
%if %{with x}
BuildRequires:  libX11-devel
BuildRequires:  libXcursor-devel
BuildRequires:  libXdamage-devel
BuildRequires:  libXext-devel
BuildRequires:  libXinerama-devel
BuildRequires:  libXv-devel
BuildRequires:  libxkbfile-devel
%endif

BuildRequires:  openssl-devel
BuildRequires:  pkgconfig(libpulse)
BuildRequires:  xmlto

Provides:       xfreerdp = %{version}-%{release}
Requires:       %{name}-libs%{?_isa} = %{?epoch}:%{version}-%{release}


%description
The xfreerdp Remote Desktop Protocol (RDP) client from the FreeRDP
project.

xfreerdp can connect to RDP servers such as Microsoft Windows
machines, xrdp and VirtualBox.


%package        libs
Summary:        Core libraries implementing the RDP protocol
Obsoletes:      %{name}-plugins < 1.1.0-0
Provides:       %{name}-plugins = %{version}-%{release}
%description    libs
libfreerdp-core can be embedded in applications.

libfreerdp-channels and libfreerdp-kbd might be convenient to use in X
applications together with libfreerdp-core.

libfreerdp-core can be extended with plugins handling RDP channels.


%package        devel
Summary:        Development files for %{name}
Requires:       %{name}-libs%{?_isa} = %{?epoch}:%{version}-%{release}
Requires:       pkgconfig


%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name} libs.


%prep

%setup -q -n freerdp-1.1.0-beta+2013071101

cat << EOF > xfreerdp.desktop
[Desktop Entry]
Type=Application
Name=X FreeRDP
NoDisplay=true
Comment=Connect to RDP server and display remote desktop
Icon=%{name}
Exec=/usr/bin/xfreerdp
Terminal=false
Categories=Network;RemoteAccess;
EOF


%build

%cmake \
        -DWITH_CUPS=ON \
        -DWITH_PCSC=OFF \
        -DWITH_PULSEAUDIO=ON \
%if %{with x}
        -DWITH_X11=ON \
        -DWITH_XCURSOR=ON \
        -DWITH_XEXT=ON \
        -DWITH_XINERAMA=ON \
        -DWITH_XKBFILE=ON \
        -DWITH_XV=ON \
%else
        -DWITH_X11=OFF \
        -DWITH_XCURSOR=OFF \
        -DWITH_XEXT=OFF \
        -DWITH_XINERAMA=OFF \
        -DWITH_XKBFILE=OFF \
        -DWITH_XV=OFF \
%endif
        -DWITH_ALSA=OFF \
        -DWITH_CUNIT=OFF \
        -DWITH_DIRECTFB=OFF \
        -DWITH_FFMPEG=OFF \
%ifarch x86_64
        -DWITH_SSE2=ON \
%else
        -DWITH_SSE2=OFF \
%endif
%ifarch armv7hl
        -DARM_FP_ABI=hard \
        -DWITH_NEON=OFF \
%endif
%ifarch armv7hnl
        -DARM_FP_ABI=hard \
        -DWITH_NEON=ON \
%endif
%ifarch armv5tel armv6l armv7l
        -DARM_FP_ABI=soft \
        -DWITH_NEON=OFF \
%endif
        -DCMAKE_INSTALL_LIBDIR:PATH=%{_lib} \
        .

make %{?_smp_mflags} V=1


%install
make install DESTDIR=$RPM_BUILD_ROOT INSTALL='install -p'

desktop-file-install --dir=$RPM_BUILD_ROOT%{_datadir}/applications xfreerdp.desktop
install -p -m 644 -D resources/FreeRDP_Icon_256px.png $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/256x256/apps/%{name}.png

rm -f %{buildroot}/%{_libdir}/*.a

%post
# This is no gtk application, but try to integrate nicely with GNOME if it is available
gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :


%post libs -p /sbin/ldconfig


%postun libs -p /sbin/ldconfig


%files
%if %{with x}
%{_bindir}/xfreerdp
%{_mandir}/man1/xfreerdp.*
%{_datadir}/applications/xfreerdp.desktop
%else
%exclude %{_datadir}/applications/xfreerdp.desktop
%endif

%{_datadir}/icons/hicolor/256x256/apps/%{name}.png


%files libs
%{_libdir}/lib*.so.*


%files devel
%{_includedir}/*/*
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/%{name}.pc
