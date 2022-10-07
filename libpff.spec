Name: libpff
Version: 20120802
Release: 1
Summary: Library to to access the Personal Folder File (PFF) and the Offline Folder File (OFF) format. PFF is used in PAB (Personal Address Book), PST (Personal Storage Table) and OST (Offline Storage Table) files.
Group: System Environment/Libraries
License: LGPL
Source: %{name}-%{version}.tar.gz
URL: http://code.google.com/p/libpff/
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
               
               

%description
libpff is a library to access the Personal Folder File (PFF) and the Offline Folder File (OFF) format. PFF is used in PAB (Personal Address Book), PST (Personal Storage Table) and OST (Offline Storage Table) files.

%package devel
Summary: Header files and libraries for developing applications for libpff
Group: Development/Libraries
Requires: libpff = %{version}-%{release}

%description devel
Header files and libraries for developing applications for libpff.

%package tools
Summary: Several tools for reading Personal Folder Files (OST, PAB and PST)
Group: Applications/System
Requires: libpff = %{version}-%{release} 
@libevt_tools_spec_build_requires@ 

%description tools
Several tools for reading Personal Folder Files (OST, PAB and PST)

%package python
Summary: Python binding for libpff
Group: System Environment/Libraries
Requires: libpff = %{version}-%{release} python
BuildRequires: python-devel

%description python
Python binding for libpff

%prep
%setup -q

%build
%configure --prefix=/usr --libdir=%{_libdir} --mandir=%{_mandir}
make %{?_smp_mflags}

%install
rm -rf ${RPM_BUILD_ROOT}
make DESTDIR=${RPM_BUILD_ROOT} install

%clean
rm -rf ${RPM_BUILD_ROOT}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README
%attr(755,root,root) %{_libdir}/*.so.*

%files devel
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README README.dll README.macosx README.mingw README.static ChangeLog
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_libdir}/pkgconfig/libpff.pc
%{_includedir}/*
%{_mandir}/man3/*

%files tools
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README
%attr(755,root,root) %{_bindir}/pffexport
%attr(755,root,root) %{_bindir}/pffinfo
%{_mandir}/man1/*

%files python
%defattr(644,root,root,755)
%attr(755,root,root) %{_libdir}/python*/site-packages/*.so.*
%{_libdir}/python*/site-packages/*.a
%{_libdir}/python*/site-packages/*.la
%{_libdir}/python*/site-packages/*.so

%changelog
* Tue Apr 24 2012 Joachim Metz <joachim.metz@gmail.com> 20120424-1
- Changes due to dependency changes

* Tue Oct 18 2011 Joachim Metz <joachim.metz@gmail.com> 20111018-1
- Update for configure changes and README files
- Added support for Python bindings

* Tue Jun 29 2010 Joachim Metz <joachim.metz@gmail.com> 20100629-1
- Clean up of previous version of spec file

