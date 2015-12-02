require 'open3'
require 'fileutils'

MRuby::Gem::Specification.new('mruby-docopt') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Terence Lee and Christophe Philemotte'
  spec.summary = 'mrbgem of docopt used for option parsing. http://docopt.org/'
  spec.add_dependency 'mruby-mtest', mgem: 'mruby-mtest'

  docopt_dir = "#{build_dir}/docopt"

  def run_command env, command
    STDOUT.sync = true
    puts "build: [exec] #{command}"
    Open3.popen2e(env, command) do |stdin, stdout, thread|
      print stdout.read
      fail "#{command} failed" if thread.value != 0
    end
  end

  FileUtils.mkdir_p build_dir

  if !File.exists?(docopt_dir)
    Dir.chdir(build_dir) do
      e = {}
      run_command e, "git clone https://github.com/docopt/docopt.cpp #{docopt_dir}"
    end
  end

  if !File.exists?("#{docopt_dir}/libdocopt_s.a")
    Dir.chdir(docopt_dir) do
      e = {
        'CC' => "#{spec.build.cc.command} #{spec.build.cc.flags.join(' ')}",
        'CXX' => "#{spec.build.cxx.command} #{spec.build.cxx.flags.join(' ')}",
        'LD' => "#{spec.build.linker.command} #{spec.build.linker.flags.join(' ')}",
        'AR' => spec.build.archiver.command
      }
      if build.kind_of?(MRuby::CrossBuild) && build.host_target && build.build_target
        if build.host_target == "x86_64-w64-mingw32" || build.host_target == "i686-w64-mingw32"
          run_command e, "sed -i '2iSET(CMAKE_SYSTEM_NAME Windows)' CMakeLists.txt"
          run_command e, "sed -i '3iSET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS \"\")' CMakeLists.txt"
          run_command e, "sed -i '4iSET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS \"\")' CMakeLists.txt"
        end
      end
      run_command e, %Q{cmake -G "Unix Makefiles"}
      if build.kind_of?(MRuby::CrossBuild) && build.host_target && build.build_target
        if build.host_target == "x86_64-apple-darwin14" || build.host_target == "i386-apple-darwin14"
          run_command e, "sed -i -e 's/-soname/-install_name/' CMakeFiles/docopt.dir/link.txt"
          run_command e, "sed -i -e 's/\\/usr\\/bin\\//\\/opt\\/osxcross\\/target\\/bin\\/x86_64-apple-darwin14-/' CMakeFiles/docopt_s.dir/link.txt"
        end
      end
      run_command e, "make"
    end
  end

  spec.cxx.include_paths << docopt_dir
  spec.cxx.include_paths << File.join(File.dirname(__FILE__), "include")
  spec.cxx.flags << "-std=c++11"
  spec.build.linker.library_paths << docopt_dir
  spec.build.linker.flags_after_libraries << "-ldocopt_s"
end
