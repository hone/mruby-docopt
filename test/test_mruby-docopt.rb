class TestMrubyDocopt < MTest::Unit::TestCase
  def test_help
    usage = <<USAGE
Naval Fate.

    Usage:
      naval_fate ship new <name>...
      naval_fate ship <name> move <x> <y> [--speed=<kn>]
      naval_fate ship shoot <x> <y>
      naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
      naval_fate (-h | --help)
      naval_fate --version

    Options:
      -h --help     Show this screen.
      --version     Show version.
      --speed=<kn>  Speed in knots [default: 10].
      --moored      Moored (anchored) mine.
      --drifting    Drifting mine.
USAGE

    argv = ["naval_fate", "-h"]

    result = Docopt.parse(usage, argv)
    assert_true result["-h"]
    assert_nil result["speed"]
  end
end

MTest::Unit.new.run
