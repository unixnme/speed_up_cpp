use std::env;
use std::fs::File;
use std::io::{BufReader, BufWriter, Read};
use std::str::FromStr;
use std::io::Write;

fn main() {
    let mut input_file = String::from("/dev/stdin");
    let mut output_file = String::from("/dev/stdout");
    let args: Vec<String> = env::args().collect();
    if args.len() >= 2 {
        input_file = args[1].clone();
    }
    if args.len() >= 3 {
        output_file = args[2].clone();
    }

    let input_file = File::open(&input_file).expect("Error opening input file");
    let output_file = File::create(&output_file).expect("Error creating output file");
    let mut ifs = BufReader::new(input_file);
    let mut ofs = BufWriter::new(output_file);

    let mut s = String::new();
    ifs.read_to_string(&mut s).expect("Error reading input file");
    let mut xs = Vec::<i64>::new();
    for x in s.split_whitespace() {
        match i64::from_str(x) {
            Ok(val) => { xs.push(val); }
            _ => { break; }
        }
    }

    xs.sort();
    for x in xs {
        writeln!(ofs, "{}", x).expect("Error writing to file");
    }
}
