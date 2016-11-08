class TestMrubyDocopt < MTest::Unit::TestCase
    USAGE = <<USAGE
Naval Fate.

    Usage:
      naval_fate ship new <name>...
      naval_fate ship <name> move <x> <y> [--speed=<kn>] [--acc=<kns>]
      naval_fate ship shoot <x> <y>
      naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
      naval_fate (-h | --help)
      naval_fate --version
      naval_fate (-o | --option)

    Options:
      -h --help     Show this screen.
      --version     Show version.
      --speed=<kn>  Speed in knots [default: 10].
      --acc=<kns>   Speed in knots per second.
      --moored      Moored (anchored) mine.
      --drifting    Drifting mine.
      -o --option   Test long and short option.
USAGE

  # make sure doesn't core dump
  def test_empty_argv
    #assert Docopt.parse(USAGE, [])
  end

  def test_bool
    argv = "naval_fate --help".split

    puts Docopt::Options

    options = Docopt.parse(USAGE, argv)
    assert_true options["--help"]
  end

=begin
  def test_string
    x    = "1"
    y    = "2"
    argv = "naval_fate ship shoot #{x} #{y}".split

    options = Docopt.parse(USAGE, argv)
    assert_true options["ship"]
    assert_true options["shoot"]
    assert_equal x, options["<x>"]
    assert_equal y, options["<y>"]
  end

  def test_short_and_long_options_are_set_when_provided
    argv = "naval_fate -o".split
    options = Docopt.parse(USAGE, argv)
    assert_true options["--option"]
    argv = "naval_fate --option".split
    options = Docopt.parse(USAGE, argv)
    assert_true options["--option"]
  end

  def test_array_string
    names = %w(enterprise mission)
    argv  = "naval_fate ship new #{names.join(" ")}".split

    options = Docopt.parse(USAGE, argv)
    assert_true options["ship"]
    assert_equal names, options["<name>"]
  end

  def test_default
    argv  = "naval_fate ship foo move 0 0".split

    options = Docopt.parse(USAGE, argv)
    assert_equal "10", options["--speed"]
  end

  def test_empty
    argv  = "naval_fate ship foo move 0 0".split

    options = Docopt.parse(USAGE, argv)
    assert_nil options["--acc"]
  end

  def test_complex
    name  = "enterprise"
    x     = "1"
    y     = "2"
    speed = "10"
    argv  = "naval_fate ship #{name} move #{x} #{y} --speed=#{speed}".split

    options = Docopt.parse(USAGE, argv)
    assert_true options["ship"]
    assert_true options["move"]
    assert_equal [name], options["<name>"]
    assert_equal x, options["<x>"]
    assert_equal y, options["<y>"]
    assert_equal speed, options["--speed"]
  end
=end
end

MTest::Unit.new.run
